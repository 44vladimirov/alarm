#pragma once

#include <string>

struct CmdArgs {
    bool infinite;
    unsigned long count;
    unsigned long timeout;
    std::string fileName;

    bool Init(int const argc, char const * const argv[]);
    void Reset();
};
