#include "common.h"

using Input = std::vector<std::string>;

Input parse_input(const char* path)
{
    return input_to_lines(path);
}

uint64_t step1(const Input& input)
{
    uint64_t total = 0;
    for (const auto& line : input)
    {
        char first = 0;
        for (auto it = line.begin();; ++it)
        {
            if (isdigit(*it))
            {
                first = *it;
                break;
            }
        }
        char last = 0;
        for (auto it = line.rbegin();; ++it)
        {
            if (isdigit(*it))
            {
                last = *it;
                break;
            }
        }
        total += (first - '0') * 10 + (last - '0');
    }
    return total;
}

static uint64_t digit_from_string(const char* s)
{
    if (!strncmp(s, "one", sizeof("one") - 1))
    {
        return '1';
    }
    else if (!strncmp(s, "two", sizeof("two") - 1))
    {
        return '2';
    }
    else if (!strncmp(s, "three", sizeof("three") - 1))
    {
        return '3';
    }
    else if (!strncmp(s, "four", sizeof("four") - 1))
    {
        return '4';
    }
    else if (!strncmp(s, "five", sizeof("five") - 1))
    {
        return '5';
    }
    else if (!strncmp(s, "six", sizeof("six") - 1))
    {
        return '6';
    }
    else if (!strncmp(s, "seven", sizeof("seven") - 1))
    {
        return '7';
    }
    else if (!strncmp(s, "eight", sizeof("eight") - 1))
    {
        return '8';
    }
    else if (!strncmp(s, "nine", sizeof("nine") - 1))
    {
        return '9';
    }
    return 0;
}

uint64_t step2(const Input& input)
{
    uint64_t total = 0;
    for (const auto& line : input)
    {
        char first = 0;
        for (auto it = line.begin();; ++it)
        {
            if (isdigit(*it))
            {
                first = *it;
                break;
            }
            uint64_t digit = digit_from_string(&*it);
            if (digit != 0)
            {
                first = digit;
                break;
            }
        }
        char last = 0;
        for (auto it = line.rbegin();; ++it)
        {
            if (isdigit(*it))
            {
                last = *it;
                break;
            }
            uint64_t digit = digit_from_string(&*it);
            if (digit != 0)
            {
                last = digit;
                break;
            }
        }
        total += (first - '0') * 10 + (last - '0');
    }
    return total;
}

int main()
{
    {
        uint64_t expected1 = 142;
        uint64_t expected2 = 281;
        Input input1 = parse_input("input/day1_provided");
        Input input2 = parse_input("input/day1_provided2");
        auto [res1, time1] = time_function(step1, input1);
        auto [res2, time2] = time_function(step2, input2);

        std::cout << "    Step1: Expected: " << expected1 << "  |  Result: " << res1 << "  |  Time: " << time1 << "  |  "
                  << (res1 == expected1 ? "OK" : "KO") << "\n";
        std::cout << "    Step2: Expected: " << expected2 << "  |  Result: " << res2 << "  |  Time: " << time2 << "  |  "
                  << (res2 == expected2 ? "OK" : "KO") << "\n";

        if (res1 != expected1 || res2 != expected2)
        {
            return 1;
        }
    }

    Input input = parse_input("input/day1");
    auto [res1, time1] = time_function(step1, input);
    auto [res2, time2] = time_function(step2, input);
    std::cout << "\n=================================\n";
    std::cout << "Step1: Result: " << res1 << "  |  Time: " << time1 << " ms\n";
    std::cout << "Step2: Result: " << res2 << "  |  Time: " << time2 << " ms\n";
}
