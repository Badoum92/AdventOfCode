#include "common.h"

const char* provided_paths[] = {"input/day10_provided"};
uint64_t provided_expected1[] = {13140};
uint64_t provided_expected2[] = {0};

const char* real_input = "input/day10";

struct Instruction
{
    std::string name;
    int32_t arg;
};

using Input = std::vector<Instruction>;

Input parse_input(const char* path)
{
    auto lines = input_to_lines(path);
    Input input;
    input.reserve(lines.size());
    for (const auto& line : lines)
    {
        auto line_split = split(line, " ");
        if (line_split.size() > 1)
        {
            input.push_back(Instruction{line_split[0], std::stoi(line_split[1])});
        }
        else
        {
            input.push_back(Instruction{line_split[0], 0});
        }
    }
    return input;
}

uint64_t step1(const Input& input)
{
    uint64_t total = 0;
    uint32_t cycle = 1;
    uint32_t X = 1;
    for (const auto& instr : input)
    {
        int32_t add = 0;
        uint32_t delay = 0;
        if (instr.name == "noop")
        {
            delay = 1;
        }
        else
        {
            add = instr.arg;
            delay = 2;
        }

        for (uint32_t i = 0; i < delay; ++i)
        {
            if (cycle == 20 || cycle == 60 || cycle == 100 || cycle == 140 || cycle == 180 || cycle == 220)
            {
                total += cycle * X;
            }
            if (cycle == 220)
            {
                return total;
            }
            cycle++;
        }
        X += add;
    }
    return total;
}

uint64_t step2(const Input& input)
{
    uint32_t cycle = 1;
    uint32_t X = 1;
    uint32_t crt_x = 0;
    for (const auto& instr : input)
    {
        int32_t add = 0;
        uint32_t delay = 0;
        if (instr.name == "noop")
        {
            delay = 1;
        }
        else
        {
            add = instr.arg;
            delay = 2;
        }

        for (uint32_t i = 0; i < delay; ++i)
        {
            if (X - 1 <= crt_x && crt_x <= X + 1)
            {
                std::cout << "#";
            }
            else
            {
                std::cout << " ";
            }
            crt_x++;
            if (cycle % 40 == 0)
            {
                crt_x = 0;
                std::cout << "\n";
            }
            cycle++;
        }
        X += add;
    }
    return 0;
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
