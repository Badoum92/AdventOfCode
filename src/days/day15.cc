#include "common.hh"

#define TURNS_STEP1 2020
#define TURNS_STEP2 30000000

static unsigned solve(const char* input_file, unsigned nb_turns)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);
    auto numbers_str = split(lines[0], ",");
    std::vector<unsigned> numbers(nb_turns, 0);
    unsigned prev_n = 0;
    unsigned next_n = 0;
    unsigned turn = 1;
    for (unsigned i = 0; i < numbers_str.size(); ++i, ++turn)
    {
        unsigned n = std::stoi(numbers_str[i]);
        next_n = numbers[n] == 0 ? 0 : turn - numbers[n];
        numbers[n] = i + 1;
    }

    for (; turn <= nb_turns; ++turn)
    {
        prev_n = next_n;
        unsigned prev_turn = numbers[next_n];
        numbers[next_n] = turn;
        next_n = prev_turn == 0 ? 0 : turn - prev_turn;
    }
    return prev_n;
}

static unsigned step1(const char* input_file)
{
    return solve(input_file, TURNS_STEP1);
}

static unsigned step2(const char* input_file)
{
    return solve(input_file, TURNS_STEP2);
}

void Day15::run()
{
    std::cout << "Day15:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day15::sanity_check()
{
    unsigned step1_expected = 436;
    unsigned step2_expected = 175594;
    bool ret = true;

    unsigned step1_got = step1(provided_file);
    if (step1_got != step1_expected)
    {
        fprintf(stderr,
                "\n    step1 sanity check failed: expected %u, got %u\n",
                step1_expected, step1_got);
        ret = false;
    }

    unsigned step2_got = step2(provided_file);
    if (step2_got != step2_expected)
    {
        fprintf(stderr,
                "\n    step2 sanity check failed: expected %u, got %u\n",
                step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
