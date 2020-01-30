#pragma once

#include <iostream>
#include <string>

class Logger {
public:
    template<typename T>
    static void info(const char* src, const T& msg) {
        std::cout << src << " | " << msg << std::endl;
    }

    template<typename T>
    static void error(const char* src, const T& msg) {
        std::cerr << src << " | " << msg << std::endl;
    }

    static void gap() {
        std::cout << std::endl;
    }
};