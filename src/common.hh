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
#include <cassert>
#include <cmath>
#include <cstdio>
#include <regex>
#include <optional>
#include <queue>
#include <deque>

class Day
{
public:
    virtual void run() = 0;
    virtual bool sanity_check() = 0;
    virtual ~Day() = default;

protected:
};

#define DAY_CLASS(N)                                                           \
    class Day##N : public Day                                                  \
    {                                                                          \
    public:                                                                    \
        void run() override;                                                   \
        bool sanity_check() override;                                          \
    private:                                                                   \
        const char* input_file = "../input/day" #N;                            \
        const char* provided_file = "../input/day" #N "_provided";             \
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
DAY_CLASS(11);
DAY_CLASS(12);
DAY_CLASS(13);
DAY_CLASS(14);
DAY_CLASS(15);
DAY_CLASS(16);
DAY_CLASS(17);
DAY_CLASS(18);
DAY_CLASS(19);
DAY_CLASS(20);
DAY_CLASS(21);
DAY_CLASS(22);
DAY_CLASS(23);
DAY_CLASS(24);
DAY_CLASS(25);

template <typename T>
std::vector<T> input_tokens_to_T(std::ifstream& input)
{
    std::vector<T> v;
    T t;
    while ((input >> t))
    {
        v.push_back(t);
    }
    return v;
}

std::vector<std::string> input_to_lines(std::ifstream& input);
std::vector<std::string> split(const std::string& s, const std::string& delim);
