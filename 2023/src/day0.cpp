#include "common.h"

using Input = std::vector<std::string>;

Input parse_input(const char* path)
{
    return input_to_lines(path);
}

uint64_t step1(const Input& input)
{
    return 1;
}

uint64_t step2(const Input& input)
{
    return 2;
}

int main()
{
    {
        uint64_t expected1 = 1;
        uint64_t expected2 = 2;
        Input input1 = parse_input("input/day0_provided");
        Input input2 = parse_input("input/day0_provided");
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
