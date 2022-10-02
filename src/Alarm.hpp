#pragma once

#include <map>
#include <string>

// util stuff
namespace alarm {
    std::string const utilName = "alarm";
    std::string const utilUsage =
        utilName + " [-r count] [-R] [-f] [-t timeout] [sound]"
        + "\n\t" + "-r count" + "\n\t\t" + "repeating alarm count times"
        + "\n\t" + "-R" + "\n\t\t" + "non-repeating alarm"
        + "\n\t" + "-f" + "\n\t\t" + "infinite repeating alarm"
        + "\n\t" + "-t timeout" + "\n\t\t" + "repeating timeout, seconds"
        + "\n\t" + "sound" + "\n\t\t" + "alarm sound file name"
        ;
}

// errors stuff
namespace alarm {
    enum class ErrCode : int {
        usage = 1,
        soundOpen,
        soundHeader,
        sndioOpen,
        sndioStart,
        sndioPlay,
    };
    std::string const errPrefix = utilName + " error: ";
    std::map <ErrCode, std::string> const errMessages = {
        {ErrCode::usage, errPrefix + "invalid usage"},
        {ErrCode::soundOpen, errPrefix + "sound file opening failed"},
        {ErrCode::soundHeader, errPrefix + "invalid sound file header"},
        {ErrCode::sndioOpen, errPrefix + "sndio playback device opening failed"},
        {ErrCode::sndioStart, errPrefix + "sndio playback device start failed"},
        {ErrCode::sndioPlay, errPrefix + "sndio playback failed"},
    };
}

// default parameters
namespace alarm {
    size_t constexpr defaultBufferSize = 4096;
    bool constexpr defaultInfiniteFlag = false;
    unsigned long constexpr defaultCount = 1;
    unsigned long constexpr defaultTimeout = 0;
    std::string const defaultFileName = "/usr/local/etc/alarm";
}
