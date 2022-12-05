#include "common.h"

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
    std::array<std::vector<char>, 10> stacks;
    std::vector<MoveOperation> moves;
};

Input parse_input(const char* path)
{
    Input input;
    auto lines = input_to_lines(path);

    size_t i = 0;
    for (; lines[i][1] != '1'; ++i)
    {
        const auto& line = lines[i];
        for (size_t j = 0; j < 10; ++j)
        {
            size_t index = 1 + j * 4;
            if (index < line.size() - 1 && line[index] != ' ')
            {
                input.stacks[j + 1].push_back(line[index]);
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

std::string string_from_stacks(const Input& input)
{
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

std::string step1(Input input)
{
    for (const auto& move : input.moves)
    {
        auto& from = input.stacks[move.from];
        auto& to = input.stacks[move.to];

        to.insert(to.end(), from.rbegin(), from.rbegin() + move.amount);
        from.erase(from.end() - move.amount, from.end());
    }
    return string_from_stacks(input);
}

std::string step2(Input input)
{
    for (const auto& move : input.moves)
    {
        auto& from = input.stacks[move.from];
        auto& to = input.stacks[move.to];

        to.insert(to.end(), from.end() - move.amount, from.end());
        from.erase(from.end() - move.amount, from.end());
    }
    return string_from_stacks(input);
}

int main()
{
    for (size_t i = 0; i < sizeof(provided_paths) / sizeof(provided_paths[0]); ++i)
    {
        Input input = parse_input(provided_paths[i]);
        auto [res1, time1] = time_function(step1, input);
        auto [res2, time2] = time_function(step2, input);

        std::cout << "Provided input #" << i << "\n";
        std::cout << "    Step1: Expected: " << provided_expected1[i] << "  |  Result: " << res1
                  << "  |  Time: " << time1 << "  |  " << (res1 == provided_expected1[i] ? "OK" : "KO") << "\n";
        std::cout << "    Step2: Expected: " << provided_expected2[i] << "  |  Result: " << res2
                  << "  |  Time: " << time2 << "  |  " << (res2 == provided_expected2[i] ? "OK" : "KO") << "\n";

        if (res1 != provided_expected1[i] || res2 != provided_expected2[i])
        {
            return 1;
        }
    }

    Input input = parse_input(real_input);
    auto [res1, time1] = time_function(step1, input);
    auto [res2, time2] = time_function(step2, input);
    std::cout << "\n=================================\n";
    std::cout << "Step1: Result: " << res1 << "  |  Time: " << time1 << " ms\n";
    std::cout << "Step2: Result: " << res2 << "  |  Time: " << time2 << " ms\n";
}
