#include "common.h"

const char* provided_paths[] = {"input/day1_provided"};
uint64_t provided_expected1[] = {24000};
uint64_t provided_expected2[] = {45000};

const char* real_input = "input/day1";

using Input = std::vector<std::string>;

Input parse_input(const char* path)
{
    return input_to_lines(path);
}

uint64_t step1(const Input& input)
{
    uint64_t max = 0;
    uint64_t current = 0;
    for (const auto& s : input)
    {
        if (s.empty())
        {
            max = std::max(current, max);
            current = 0;
        }
        else
        {
            current += std::stoull(s);
        }
    }
    max = std::max(current, max);
    return max;
}

uint64_t step2(const Input& input)
{
    std::array<uint64_t, 4> max = {0, 0, 0, 0};
    for (const auto& s : input)
    {
        if (s.empty())
        {
            std::sort(max.begin(), max.end());
            max[0] = 0;
        }
        else
        {
            max[0] += std::stoull(s);
        }
    }
    std::sort(max.begin(), max.end());
    return max[1] + max[2] + max[3];
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
