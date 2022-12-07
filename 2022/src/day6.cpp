#include "common.h"

const char* provided_paths[] = {"input/day6_provided"};
uint64_t provided_expected1[] = {7};
uint64_t provided_expected2[] = {19};

const char* real_input = "input/day6";

using Input = std::string;

Input parse_input(const char* path)
{
    return input_to_lines(path)[0];
}

bool no_duplicates(const std::string& str, size_t index, size_t size)
{
    for (size_t i = index; i < index + size; ++i)
    {
        for (size_t j = index; j < index + size; ++j)
        {
            if (i == j)
            {
                continue;
            }
            if (str[i] == str[j])
            {
                return false;
            }
        }
    }
    return true;
}

uint64_t step1(const Input& input)
{
    size_t size = 4;
    for (size_t i = 0; i < input.size(); ++i)
    {
        if (no_duplicates(input, i, size))
        {
            return i + size;
        }
    }
    return 0;
}

uint64_t step2(const Input& input)
{
    size_t size = 14;
    for (size_t i = 0; i < input.size(); ++i)
    {
        if (no_duplicates(input, i, size))
        {
            return i + size;
        }
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
