#pragma once

/*
    File to declare some macros that might be useful to debug
*/

#include <iostream>

#define log(msg) (std::cout << "\033[33m" << msg << "\033[0m" << '\n')