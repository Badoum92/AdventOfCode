#pragma once

#include <cstdint>
#include <cstdio>
#include <chrono>
#include <cctype>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <iostream>

template <typename F, typename... Args>
inline auto time_function(const F& function, Args&&... args)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto ret = function(std::forward<Args>(args)...);
    auto stop = std::chrono::high_resolution_clock::now();
    uint64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    return std::make_pair(ret, time);
}

template <typename T>
inline std::vector<T> input_to_tokens(const char* path)
{
    std::ifstream input(path);
    std::vector<T> v;
    T t;
    while ((input >> t))
    {
        v.push_back(t);
    }
    return v;
}

inline std::vector<std::string> input_to_lines(const char* path)
{
    std::ifstream input(path);
    std::vector<std::string> v;
    std::string s;
    while (std::getline(input, s))
    {
        v.push_back(std::move(s));
    }
    return v;
}

inline std::vector<std::string> split(const std::string& s, const std::string& delim)
{
    std::vector<std::string> v;
    size_t cur_idx = 0;
    size_t delim_idx = 0;
    while (delim_idx != s.npos)
    {
        delim_idx = s.find(delim, cur_idx);
        std::string sub = s.substr(cur_idx, delim_idx - cur_idx);
        if (!sub.empty())
            v.push_back(std::move(sub));
        cur_idx = delim_idx + delim.size();
    }
    return v;
}
