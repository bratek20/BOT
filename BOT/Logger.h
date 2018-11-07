#pragma once

#include <iostream>
#include <string>

class Logger {
public:
    template<typename T>
    static void info(char* src, const T& msg) {
        std::cout << src << " | " << msg << std::endl;
    }

    template<typename T>
    static void error(char* src, const T& msg) {
        std::cerr << src << " | " << msg << std::endl;
    }
};