#include "common.h"

#include <array>

const char* provided_paths[] = {"input/day3_provided"};
uint64_t provided_expected1[] = {157};
uint64_t provided_expected2[] = {70};

const char* real_input = "input/day3";

constexpr size_t MAX_PRIO = 53;

using Input = std::vector<std::string>;

Input parse_input(const char* path)
{
    return input_to_lines(path);
}

uint64_t priority(char c)
{
    return std::islower(c) ? (c - 'a' + 1) : (c - 'A' + 27);
}

uint64_t step1(const Input& input)
{
    uint64_t total = 0;
    for (const auto& s : input)
    {
        bool compartment1[MAX_PRIO] = {false};
        bool compartment2[MAX_PRIO] = {false};

        for (size_t i = 0; i < s.size() / 2; ++i)
        {
            uint64_t prio = priority(s[i]);
            compartment1[prio] = true;
        }

        for (size_t i = s.size() / 2; i < s.size(); ++i)
        {
            uint64_t prio = priority(s[i]);
            if (compartment1[prio] && !compartment2[prio])
            {
                compartment2[prio] = true;
                total += prio;
            }
        }
    }
    return total;
}

uint64_t group_badge_priority(const Input& input, size_t line)
{
    int item_count[MAX_PRIO] = {0};
    bool items[MAX_PRIO * 3] = {false};
    for (size_t i = 0; i < 3; ++i)
    {
        for (char c : input[line + i])
        {
            uint64_t prio = priority(c);
            if (items[i * MAX_PRIO + prio])
            {
                continue;
            }
            items[i * MAX_PRIO + prio] = true;
            ++item_count[prio];
            if (item_count[prio] == 3)
            {
                return prio;
            }
        }
    }
    return 0;
}

uint64_t step2(const Input& input)
{
    uint64_t total = 0;
    for (size_t line = 0; line < input.size(); line += 3)
    {
        total += group_badge_priority(input, line);
    }
    return total;
}

int main()
{
    for (size_t i = 0; i < sizeof(provided_paths) / sizeof(provided_paths[0]); ++i)
    {
        Input input = parse_input(provided_paths[i]);
        auto [res1, time1] = time_function(step1, input);
        auto [res2, time2] = time_function(step2, input);

        printf("Provided input #%zu:\n", i);
        printf("    Step1: Expected: %llu  |  Result: %llu  |  Time: %llu ms  |  %s\n", provided_expected1[i], res1,
               time1, res1 == provided_expected1[i] ? "OK" : "KO");
        printf("    Step2: Expected: %llu  |  Result: %llu  |  Time: %llu ms  |  %s\n", provided_expected2[i], res2,
               time2, res2 == provided_expected2[i] ? "OK" : "KO");

        if (res1 != provided_expected1[i] || res2 != provided_expected2[i])
        {
            return 1;
        }
    }

    Input input = parse_input(real_input);
    auto [res1, time1] = time_function(step1, input);
    auto [res2, time2] = time_function(step2, input);
    printf("\n=================================\n");
    printf("Step1: Result: %llu  |  Time: %llu ms\n", res1, time1);
    printf("Step2: Result: %llu  |  Time: %llu ms\n", res2, time2);
}
