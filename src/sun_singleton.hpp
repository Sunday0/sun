/*
 * 单例模板类
 * 继承即可
 */

#pragma once

template <typename T>
class sun_singleton {
public:
    static auto get_instance()->decltype(T*)
    {
        static T m;
        return &m;
    }
 };