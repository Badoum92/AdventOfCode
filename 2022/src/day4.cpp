#include "common.h"

#include <string>

const char* provided_paths[] = {"input/day4_provided"};
uint64_t provided_expected1[] = {2};
uint64_t provided_expected2[] = {4};

const char* real_input = "input/day4";

struct ElfPair
{
    uint64_t begin1;
    uint64_t end1;

    uint64_t begin2;
    uint64_t end2;
};

using Input = std::vector<ElfPair>;

Input parse_input(const char* path)
{
    Input ret;
    auto lines = input_to_lines(path);
    ret.resize(lines.size());
    for (size_t i = 0; i < lines.size(); ++i)
    {
        const auto& line = lines[i];
        auto& pair = ret[i];
        sscanf(line.c_str(), "%llu-%llu,%llu-%llu", &pair.begin1, &pair.end1, &pair.begin2, &pair.end2);
    }
    return ret;
}

bool full_overlap(const ElfPair& pair)
{
    bool a = pair.begin1 <= pair.begin2 && pair.end1 >= pair.end2;
    bool b = pair.begin2 <= pair.begin1 && pair.end2 >= pair.end1;
    return a || b;
}

bool between(uint64_t value, uint64_t a, uint64_t b)
{
    return a <= value && value <= b;
}

bool any_overlap(const ElfPair& pair)
{
    bool a = between(pair.begin1, pair.begin2, pair.end2) || between(pair.end1, pair.begin2, pair.end2);
    bool b = between(pair.begin2, pair.begin1, pair.end1) || between(pair.end2, pair.begin1, pair.end1);
    return a || b;
}

uint64_t step1(const Input& input)
{
    uint64_t count = 0;
    for (const auto& pair : input)
    {
        count += full_overlap(pair);
    }
    return count;
}

uint64_t step2(const Input& input)
{
    uint64_t count = 0;
    for (const auto& pair : input)
    {
        count += any_overlap(pair);
    }
    return count;
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
