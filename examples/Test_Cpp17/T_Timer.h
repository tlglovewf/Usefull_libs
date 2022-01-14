/* 2022.01.14 by tuligen*/
#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <functional>

namespace CommUtils
{
    /// <summary>
    /// 定时器
    /// </summary>
    class Timer {
        std::atomic<bool> active{ true };

    public:
        void setTimeout(std::function<void()> function, int delay);
        void setInterval(std::function<void()> function, int interval);
        void stop();

    };
    /// <summary>
    /// 时间消耗完毕后触发
    /// </summary>
    /// <param name="function">执行函数</param>
    /// <param name="delay">时间(ms)</param>
    void Timer::setTimeout(std::function<void()> function, int delay) {
        active = true;
        std::thread t([=]() {
            if (!active.load()) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            if (!active.load()) return;
            function();
            });
        t.detach();
    }

    /// <summary>
    /// 定时触发
    /// </summary>
    /// <param name="function">执行函数</param>
    /// <param name="interval">时间间隔(ms)</param>
    void Timer::setInterval(std::function<void()> function, int interval) {
        active = true;
        std::thread t([=]() {
            while (active.load()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(interval));
                if (!active.load()) return;
                function();
            }
            });
        t.detach();
    }

    void Timer::stop() {
        active = false;
    }
}



