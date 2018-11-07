#pragma once
#include <thread>
#include <Windows.h>
#include <functional>
#include <chrono>
#include <array>
#include <vector>

class Input {
    using Callback = std::function<void()>;
    enum PredefinedKeys {
        LeftMouseButton = 1,
        RightMouseButton = 2,

    };

public:
    Input();
    ~Input();

    void onLeftMouseClick(Callback cb = {});
    void onRightMouseClick(Callback cb = {});
    void onKeyPressed(char key, Callback cb = {});
    void onAnyKeyPressed(std::function<void(char)> cb);

private:
    static void run();
    static void prep();

    static bool isPressed(char key);
    static void perform(char key);

    static const int MAX_KEYS = 256;
    static std::array<std::vector<Callback>, MAX_KEYS> _callbacks;
    static std::function<void(char)> _anyKeyPressedCallback;
    static std::array<std::chrono::time_point<std::chrono::system_clock>, MAX_KEYS> _lastPerform;

    static bool _shouldRun;
    std::thread _t;
};
