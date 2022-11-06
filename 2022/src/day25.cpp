#include "common.h"

#include <string>

const char* provided_paths[] = {"input/day0_provided"};
uint64_t provided_expected1[] = {123};
uint64_t provided_expected2[] = {124};

const char* real_input = "input/day0";

using Input = std::vector<std::string>;

Input parse_input(const char* path)
{
    return input_to_lines(path);
}

uint64_t step1(const Input& input)
{
    return std::stoull(input[0]);
}

uint64_t step2(const Input& input)
{
    return std::stoull(input[0]) + 1;
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
