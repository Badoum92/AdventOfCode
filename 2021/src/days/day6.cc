#include "common.hh"

static std::vector<uint64_t> parse_input(std::ifstream& input)
{
    std::string line;
    std::getline(input, line);
    auto fishes_str = split(line, ",");

    std::vector<uint64_t> fishes(9);
    for (size_t i = 0; i < fishes_str.size(); ++i)
    {
        size_t idx = std::stoull(fishes_str[i]);
        ++fishes[idx];
    }
    return fishes;
}

static uint64_t simulate(std::vector<uint64_t>& fishes, size_t days)
{
    for (size_t d = 0; d < days; ++d)
    {
        uint64_t tmp = fishes[0];
        std::rotate(fishes.begin(), fishes.begin() + 1, fishes.end());
        fishes[6] += tmp;
    }
    return std::accumulate(fishes.begin(), fishes.end(), 0ULL);
}

static uint64_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto fishes = parse_input(input);
    return simulate(fishes, 80);
}

static uint64_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto fishes = parse_input(input);
    return simulate(fishes, 256);
}

void Day6::run()
{
    std::cout << "Day6:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;

    auto t0 = std::chrono::steady_clock::now();
    auto s1 = step1("../input/day6");
    auto t1 = std::chrono::steady_clock::now();
    auto s2 = step2("../input/day6");
    auto t2 = std::chrono::steady_clock::now();

    auto t_s1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto t_s2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "    step1: " << s1 << " (" << t_s1 << " ms)\n";
    std::cout << "    step2: " << s2 << " (" << t_s2 << " ms)\n\n";
}

bool Day6::sanity_check()
{
    uint64_t step1_expected = 5934;
    uint64_t step2_expected = 26984457539ULL;
    bool ret = true;

    uint64_t step1_got = step1("../input/day6_provided");
    if (step1_got != step1_expected)
    {
        fprintf(stderr, "\n    step1 sanity check failed: expected %llu, got %llu\n", step1_expected, step1_got);
        ret = false;
    }

    uint64_t step2_got = step2("../input/day6_provided");
    if (step2_got != step2_expected)
    {
        fprintf(stderr, "\n    step2 sanity check failed: expected %llu, got %llu\n", step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
