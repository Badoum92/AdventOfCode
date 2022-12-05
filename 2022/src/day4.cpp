#include "common.h"

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

bool any_overlap(const ElfPair& pair)
{
    return pair.end1 >= pair.begin2 && pair.begin1 <= pair.end2;
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
