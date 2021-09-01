#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <string>

#include "anyoption.h"

#pragma once

struct in_args {
    std::string workdir;
    std::string input_path;
    std::string output_path;
    bool verbose{false};
    unsigned int threads{1};
};

#endif
