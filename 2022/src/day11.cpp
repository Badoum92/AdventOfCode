#include "common.h"

#include <functional>

const char* provided_paths[] = {"input/day11_provided"};
int64_t provided_expected1[] = {10605};
int64_t provided_expected2[] = {2713310158};

const char* real_input = "input/day11";

struct Monkey
{
    std::vector<int64_t> items;
    std::function<int64_t(int64_t)> operation;
    std::function<int64_t(int64_t)> test;
    int64_t divisor = 0;
    int64_t inspect_times = 0;
};

using Input = std::vector<Monkey>;

Input parse_input(const char* path)
{
    Input input;
    auto lines = input_to_lines(path);
    for (size_t i = 0; i < lines.size(); ++i)
    {
        Monkey monkey;
        ++i;
        // Starting items
        {
            const auto& line = lines[i];
            auto line_split = split(line, ": ");
            auto item_split = split(line_split[1], ", ");
            for (const auto& item : item_split)
            {
                monkey.items.push_back(std::stoul(item));
            }
        }
        ++i;
        // Operation
        {
            const auto& line = lines[i];
            auto line_split = split(line, " = ");
            auto op_split = split(line_split[1], " ");
            monkey.operation = [op_split](int64_t item) {
                int64_t op1 = op_split[0] == "old" ? item : std::stoul(op_split[0]);
                int64_t op2 = op_split[2] == "old" ? item : std::stoul(op_split[2]);
                if (op_split[1] == "+")
                {
                    return op1 + op2;
                }
                else if (op_split[1] == "*")
                {
                    return op1 * op2;
                }
                return int64_t(0);
            };
        }
        ++i;
        // Test
        {
            const auto& line_test = lines[i++];
            const auto& line_true = lines[i++];
            const auto& line_false = lines[i++];
            auto line_test_split = split(line_test, " ");
            auto line_true_split = split(line_true, " ");
            auto line_false_split = split(line_false, " ");
            int64_t test_div = std::stoul(line_test_split.back());
            int64_t test_true = std::stoul(line_true_split.back());
            int64_t test_false = std::stoul(line_false_split.back());
            monkey.divisor = test_div;
            monkey.test = [test_div, test_true, test_false](int64_t item) {
                return item % test_div == 0 ? test_true : test_false;
            };
        }
        input.push_back(monkey);
    }
    return input;
}

void play_round1(Input& input)
{
    for (auto& monkey : input)
    {
        monkey.inspect_times += monkey.items.size();
        for (int64_t item : monkey.items)
        {
            item = monkey.operation(item) / 3;
            input[monkey.test(item)].items.push_back(item);
        }
        monkey.items.clear();
    }
}

int64_t step1(Input input)
{
    for (size_t i = 0; i < 20; ++i)
    {
        play_round1(input);
    }
    std::sort(input.begin(), input.end(), [&](const Monkey& a, const Monkey& b) {
        return a.inspect_times > b.inspect_times;
    });
    return input[0].inspect_times * input[1].inspect_times;
}

void play_round2(Input& input, int64_t global_modulo)
{
    for (auto& monkey : input)
    {
        monkey.inspect_times += monkey.items.size();
        for (int64_t item : monkey.items)
        {
            item = monkey.operation(item);
            input[monkey.test(item)].items.push_back(item % global_modulo);
        }
        monkey.items.clear();
    }
}

int64_t step2(Input input)
{
    int64_t global_modulo = 1;
    for (const auto& monkey : input)
    {
        global_modulo *= monkey.divisor;
    }
    for (size_t i = 0; i < 10000; ++i)
    {
        play_round2(input, global_modulo);
    }
    std::sort(input.begin(), input.end(), [&](const Monkey& a, const Monkey& b) {
        return a.inspect_times > b.inspect_times;
    });
    return int64_t(input[0].inspect_times) * int64_t(input[1].inspect_times);
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
