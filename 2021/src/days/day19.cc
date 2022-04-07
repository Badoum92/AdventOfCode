#include "common.hh"

static uint32_t step1(const char* input_file)
{
    std::ifstream input(input_file);

    return 0;
}

static uint32_t step2(const char* input_file)
{
    std::ifstream input(input_file);

    return 0;
}

void Day19::run()
{
    std::cout << "Day19:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;

    auto t0 = std::chrono::steady_clock::now();
    auto s1 = step1("../input/day19");
    auto t1 = std::chrono::steady_clock::now();
    auto s2 = step2("../input/day19");
    auto t2 = std::chrono::steady_clock::now();

    auto t_s1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto t_s2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "    step1: " << s1 << " (" << t_s1 << " ms)\n";
    std::cout << "    step2: " << s2 << " (" << t_s2 << " ms)\n\n";
}

bool Day19::sanity_check()
{
    uint32_t step1_expected = 79;
    uint32_t step2_expected = 0;
    bool ret = true;

    uint32_t step1_got = step1("../input/day19_provided");
    if (step1_got != step1_expected)
    {
        fprintf(stderr, "\n    step1 sanity check failed: expected %u, got %u\n", step1_expected, step1_got);
        ret = false;
    }

    uint32_t step2_got = step2("../input/day19_provided");
    if (step2_got != step2_expected)
    {
        fprintf(stderr, "\n    step2 sanity check failed: expected %u, got %u\n", step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
