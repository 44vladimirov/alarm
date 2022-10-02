#include "Alarm.hpp"
#include "CmdArgs.hpp"

#include <SndioPlayback.hpp>

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <istream>
#include <thread>
#include <vector>

struct RiffWaveHeader {
    uint32_t tag;
    uint32_t fileSize;
    uint32_t type;
    uint32_t format;
    uint32_t formatSize;
    uint16_t formatType;
    uint16_t channels;
    uint32_t sampleRate;
    uint32_t bytesPerSecond;
    uint16_t align;
    uint16_t bitsPerSample;
    uint32_t data;
    uint32_t dataSize;
};

static uint32_t FourCC(char const code[4]) {
    return (code[3] << 24) | (code[2] << 16) | (code[1] << 8) | code[0];
}

static bool CheckHeader(RiffWaveHeader const& header) {
    return header.tag == FourCC("RIFF")
        && header.type == FourCC("WAVE")
        && header.format == FourCC("fmt ")
        && header.formatType == 1 // PCM
        && header.data == FourCC("data") // data follows header
        ;
}

static bool PlayStream(std::istream & stream, sndio::SndioPlaybackDevice & device) {
    static std::vector<char> buffer(alarm::defaultBufferSize);
    bool result = false;

    for (result = stream.good(); result && stream.good(); stream.peek()) {
        size_t const size = stream.readsome(buffer.data(), buffer.size());
        result = device.Play(buffer.data(), size);
    }

    return result;
}

static void terminate(alarm::ErrCode const& code, std::string const& message = {}) {
    std::cerr << alarm::errMessages.at(code) << std::endl;

    if (!message.empty()) {
        std::cout << message << std::endl;
    }

    std::exit(static_cast<int>(code));
}

int main(int const argc, char const * const argv[]) {
    CmdArgs args;
    if (!args.Init(argc, argv)) {
        terminate(alarm::ErrCode::usage, alarm::utilUsage);
    }

    std::ifstream soundStream(args.fileName, std::ios::binary);
    if (!soundStream.is_open()) {
        terminate(alarm::ErrCode::soundOpen);
    }
    RiffWaveHeader header;
    soundStream.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (!soundStream.good() || !CheckHeader(header)) {
        terminate(alarm::ErrCode::soundHeader);
    }

    sndio::SndioPlaybackDevice playbackDevice;
    if (!playbackDevice.Open()) {
        terminate(alarm::ErrCode::sndioOpen);
    }
    sndio::AudioFormat const format(header.bitsPerSample, header.sampleRate, header.channels);
    if (!playbackDevice.Start(format)) {
        terminate(alarm::ErrCode::sndioStart);
    }

    for (unsigned long count = 0; args.infinite || count < args.count; ++count) {
        soundStream.clear();
        soundStream.seekg(sizeof(RiffWaveHeader));
        if (!PlayStream(soundStream, playbackDevice)) {
            terminate(alarm::ErrCode::sndioPlay);
        }

        bool const lastIteration = !args.infinite && args.count - count == 1;
        if (!lastIteration && args.timeout) {
            std::this_thread::sleep_for(std::chrono::seconds(args.timeout));
        }
    }

    return 0;
}
