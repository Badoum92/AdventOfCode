#include "common.hh"

static int32_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto commands = input_to_tokens<std::string>(input);

    int32_t x = 0;
    int32_t y = 0;

    for (size_t i = 0; i < commands.size(); i += 2)
    {
        int32_t n = commands[i + 1][0] - '0';

        if (commands[i][0] == 'f')
        {
            x += n;
        }
        else if (commands[i][0] == 'u')
        {
            y -= n;
        }
        else if (commands[i][0] == 'd')
        {
            y += n;
        }
    }

    return x * y;
}

static int32_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto commands = input_to_tokens<std::string>(input);

    int32_t x = 0;
    int32_t y = 0;
    int32_t aim = 0;

    for (size_t i = 0; i < commands.size(); i += 2)
    {
        int32_t n = commands[i + 1][0] - '0';

        if (commands[i][0] == 'f')
        {
            x += n;
            y += aim * n;
        }
        else if (commands[i][0] == 'u')
        {
            aim -= n;
        }
        else if (commands[i][0] == 'd')
        {
            aim += n;
        }
    }

    return x * y;
}

void Day2::run()
{
    std::cout << "Day2:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;

    auto t0 = std::chrono::steady_clock::now();
    auto s1 = step1("../input/day2");
    auto t1 = std::chrono::steady_clock::now();
    auto s2 = step2("../input/day2");
    auto t2 = std::chrono::steady_clock::now();

    auto t_s1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto t_s2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "    step1: " << s1 << " (" << t_s1 << " ms)\n";
    std::cout << "    step2: " << s2 << " (" << t_s2 << " ms)\n\n";
}

bool Day2::sanity_check()
{
    uint32_t step1_expected = 150;
    uint32_t step2_expected = 900;
    bool ret = true;

    uint32_t step1_got = step1("../input/day2_provided");
    if (step1_got != step1_expected)
    {
        fprintf(stderr, "\n    step1 sanity check failed: expected %u, got %u\n", step1_expected, step1_got);
        ret = false;
    }

    uint32_t step2_got = step2("../input/day2_provided");
    if (step2_got != step2_expected)
    {
        fprintf(stderr, "\n    step2 sanity check failed: expected %u, got %u\n", step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
