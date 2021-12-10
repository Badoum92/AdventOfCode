#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <algorithm>
#include <string>
#include <utility>
#include <tuple>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <array>
#include <numeric>
#include <chrono>
#include <regex>
#include <optional>
#include <queue>
#include <deque>
#include <memory>
#include <chrono>
#include <bitset>

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdint>

class Day
{
public:
    virtual void run() = 0;
    virtual bool sanity_check() = 0;
    virtual ~Day() = default;
};

#define DAY_CLASS(N)                                                           \
    class Day##N : public Day                                                  \
    {                                                                          \
    public:                                                                    \
        void run() override;                                                   \
        bool sanity_check() override;                                          \
    }

DAY_CLASS(1);
DAY_CLASS(2);
DAY_CLASS(3);
DAY_CLASS(4);
DAY_CLASS(5);
DAY_CLASS(6);
DAY_CLASS(7);
DAY_CLASS(8);
DAY_CLASS(9);
DAY_CLASS(10);
// DAY_CLASS(11);
// DAY_CLASS(12);
// DAY_CLASS(13);
// DAY_CLASS(14);
// DAY_CLASS(15);
// DAY_CLASS(16);
// DAY_CLASS(17);
// DAY_CLASS(18);
// DAY_CLASS(19);
// DAY_CLASS(20);
// DAY_CLASS(21);
// DAY_CLASS(22);
// DAY_CLASS(23);
// DAY_CLASS(24);
// DAY_CLASS(25);

template <typename T>
std::vector<T> input_to_tokens(std::ifstream& input)
{
    std::vector<T> v;
    T t;
    while ((input >> t))
    {
        v.push_back(t);
    }
    return v;
}

inline std::vector<std::string> input_to_lines(std::ifstream& input)
{
    std::vector<std::string> v;
    std::string s;
    while (std::getline(input, s))
    {
        v.push_back(s);
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
            v.push_back(sub);
        cur_idx = delim_idx + delim.size();
    }
    return v;
}
