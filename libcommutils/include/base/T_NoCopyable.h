/* 2022.01.14 by tuligen*/
#pragma once

namespace CommUtils
{
    /// <summary>
    /// ¿½±´½ûÓÃ»ùÀà
    /// </summary>
    class noncopyable
    {
    protected:
        noncopyable(void) = default;
        ~noncopyable(void) = default;
    public:
        noncopyable(const noncopyable&) = delete;
        noncopyable& operator=(const noncopyable&) = delete;
    };
}