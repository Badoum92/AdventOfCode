#include "common.h"

#include <string>

const char* provided_paths[] = {"input/day2_provided"};
uint64_t provided_expected1[] = {15};
uint64_t provided_expected2[] = {12};

const char* real_input = "input/day2";

#define ROCK     1
#define PAPER    2
#define SCISSORS 3

#define LOSE 0
#define DRAW 3
#define WIN  6

using Input = std::vector<std::string>;

uint64_t score1[3][3] = {
    {ROCK + DRAW, PAPER + WIN, SCISSORS + LOSE},
    {ROCK + LOSE, PAPER + DRAW, SCISSORS + WIN},
    {ROCK + WIN, PAPER + LOSE, SCISSORS + DRAW}
};

uint64_t score2[3][3] = {
    {LOSE + SCISSORS, LOSE + ROCK, LOSE + PAPER},
    {DRAW + ROCK, DRAW + PAPER, DRAW + SCISSORS},
    {WIN + PAPER, WIN + SCISSORS, WIN + ROCK}
};

Input parse_input(const char* path)
{
    return input_to_lines(path);
}

uint64_t step1(const Input& input)
{
    uint64_t total = 0;
    for (const auto& s : input)
    {
        total += score1[s[0] - 'A'][s[2] - 'X'];
    }
    return total;
}

uint64_t step2(const Input& input)
{
    uint64_t total = 0;
    for (const auto& s : input)
    {
        total += score2[s[2] - 'X'][s[0] - 'A'];
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
