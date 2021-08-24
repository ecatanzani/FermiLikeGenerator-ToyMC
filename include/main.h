#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <string>

#include "anyoption.h"

#pragma once

struct in_args {
    std::string workdir;
    std::string output_path:
    bool verbose{false};
};

#endif
