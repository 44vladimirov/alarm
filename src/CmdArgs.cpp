#include "Alarm.hpp"
#include "CmdArgs.hpp"

bool CmdArgs::Init(int const argc, char const * const argv[]) {
    Reset();

    for (int i = 1; i < argc; ) {
        if (!argv[i]) {
            return false;
        }
        std::string arg = argv[i++];

        if (!arg.empty() && arg[0] != '-') {
            fileName = arg;
            continue;
        }
        else if (arg == "-R") {
            count = 1;
            infinite = false;
            continue;
        }
        else if (arg == "-f") {
            infinite = true;
            continue;
        }

        if (!argv[i]) {
            return false;
        }
        std::string value = argv[i++];

        if (arg == "-r") {
            infinite = false;
            try {
                count = std::stoul(value);
            }
            catch (...) {
                return false;
            }
        }
        else if (arg == "-t") {
            try {
                timeout = std::stoul(value);
            }
            catch (...) {
                return false;
            }
        }
        else {
            return false;
        }
    }

    return true;
}

void CmdArgs::Reset() {
    infinite = alarm::defaultInfiniteFlag;
    count = alarm::defaultCount;
    timeout = alarm::defaultTimeout;
    fileName = alarm::defaultFileName;
}
