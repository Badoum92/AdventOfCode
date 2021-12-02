#include "common.hh"

static uint32_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto depths = input_to_tokens<uint32_t>(input);

    uint32_t increase_count = 0;

    for (size_t i = 1; i < depths.size(); ++i)
    {
        increase_count += depths[i] > depths[i - 1];
    }

    return increase_count;
}

static uint32_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto depths = input_to_tokens<uint32_t>(input);

    uint32_t increase_count = 0;

    for (size_t i = 3; i < depths.size(); ++i)
    {
        uint32_t sum1 = depths[i] + depths[i - 1] + depths[i - 2];
        uint32_t sum2 = depths[i - 1] + depths[i - 2] + depths[i - 3];
        increase_count += sum1 > sum2;
    }

    return increase_count;
}

void Day1::run()
{
    std::cout << "Day1:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;

    auto t0 = std::chrono::steady_clock::now();
    auto s1 = step1("../input/day1");
    auto t1 = std::chrono::steady_clock::now();
    auto s2 = step2("../input/day1");
    auto t2 = std::chrono::steady_clock::now();

    auto t_s1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto t_s2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "    step1: " << s1 << " (" << t_s1 << " ms)\n";
    std::cout << "    step2: " << s2 << " (" << t_s2 << " ms)\n\n";
}

bool Day1::sanity_check()
{
    uint32_t step1_expected = 7;
    uint32_t step2_expected = 5;
    bool ret = true;

    uint32_t step1_got = step1("../input/day1_provided");
    if (step1_got != step1_expected)
    {
        fprintf(stderr, "\n    step1 sanity check failed: expected %u, got %u\n", step1_expected, step1_got);
        ret = false;
    }

    uint32_t step2_got = step2("../input/day1_provided");
    if (step2_got != step2_expected)
    {
        fprintf(stderr, "\n    step2 sanity check failed: expected %u, got %u\n", step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
