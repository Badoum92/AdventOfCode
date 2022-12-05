#include "common.h"

#include <string>

const char* provided_paths[] = {"input/day5_provided"};
const char* provided_expected1[] = {"CMZ"};
const char* provided_expected2[] = {"MCD"};

const char* real_input = "input/day5";

struct MoveOperation
{
    uint32_t amount;
    uint32_t from;
    uint32_t to;
};

struct Input
{
    std::vector<std::vector<char>> stacks;
    std::vector<MoveOperation> moves;
};

Input parse_input(const char* path)
{
    Input input;
    input.stacks.resize(10);
    auto lines = input_to_lines(path);

    size_t i = 0;
    for (; i < lines.size(); ++i)
    {
        const auto& line = lines[i];
        if (line[1] == '1')
        {
            i += 2;
            break;
        }

        for (size_t j = 0; j < 10; ++j)
        {
            if (2 + j * 4 < line.size() && line[1 + j * 4] != ' ')
            {
                input.stacks[j + 1].push_back(line[1 + j * 4]);
            }
        }
    }

    for (auto& stack : input.stacks)
    {
        std::reverse(stack.begin(), stack.end());
    }

    for (; i < lines.size(); ++i)
    {
        const auto& line = lines[i];
        MoveOperation& move = input.moves.emplace_back();
        sscanf(line.c_str(), "move %u from %u to %u", &move.amount, &move.from, &move.to);
    }

    return input;
}

std::string step1(Input& input)
{
    for (const auto& move : input.moves)
    {
        for (uint32_t i = 0; i < move.amount; ++i)
        {
            input.stacks[move.to].push_back(input.stacks[move.from].back());
            input.stacks[move.from].pop_back();
        }
    }

    std::string res;
    for (const auto& stack : input.stacks)
    {
        if (!stack.empty())
        {
            res += stack.back();
        }
    }
    return res;
}

std::string step2(Input& input)
{
    for (const auto& move : input.moves)
    {
        for (uint32_t i = 0; i < move.amount; ++i)
        {
            input.stacks[move.to].push_back(input.stacks[move.from][input.stacks[move.from].size() - (move.amount - i)]);
        }
        for (uint32_t i = 0; i < move.amount; ++i)
        {
            input.stacks[move.from].pop_back();
        }
    }

    std::string res;
    for (const auto& stack : input.stacks)
    {
        if (!stack.empty())
        {
            res += stack.back();
        }
    }
    return res;
}

int main()
{
    for (size_t i = 0; i < sizeof(provided_paths) / sizeof(provided_paths[0]); ++i)
    {
        Input input = parse_input(provided_paths[i]);
        auto [res1, time1] = time_function(step1, input);
        input = parse_input(provided_paths[i]);
        auto [res2, time2] = time_function(step2, input);

        printf("Provided input #%zu:\n", i);
        printf("    Step1: Expected: %s  |  Result: %s  |  Time: %llu ms  |  %s\n", provided_expected1[i], res1.c_str(),
               time1, res1 == provided_expected1[i] ? "OK" : "KO");
        printf("    Step2: Expected: %s  |  Result: %s  |  Time: %llu ms  |  %s\n", provided_expected2[i], res2.c_str(),
               time2, res2 == provided_expected2[i] ? "OK" : "KO");

        if (res1 != provided_expected1[i] || res2 != provided_expected2[i])
        {
            return 1;
        }
    }

    Input input = parse_input(real_input);
    auto [res1, time1] = time_function(step1, input);
    input = parse_input(real_input);
    auto [res2, time2] = time_function(step2, input);
    printf("\n=================================\n");
    printf("Step1: Result: %s  |  Time: %llu ms\n", res1.c_str(), time1);
    printf("Step2: Result: %s  |  Time: %llu ms\n", res2.c_str(), time2);
}
