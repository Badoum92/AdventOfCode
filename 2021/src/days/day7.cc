#include "common.hh"

static std::vector<int64_t> parse_input(std::ifstream& input)
{
    std::string line;
    std::getline(input, line);
    auto positions_str = split(line, ",");

    std::vector<int64_t> positions;
    positions.reserve(positions_str.size());
    std::transform(positions_str.begin(), positions_str.end(), std::back_inserter(positions),
                   [&](const std::string& position_str) { return std::stoi(position_str); });
    return positions;
}

int64_t vector_median(std::vector<int64_t>& v)
{
    size_t n = v.size() / 2;
    nth_element(v.begin(), v.begin() + n, v.end());
    return v[n];
}

static int64_t distance(int64_t position, int64_t target)
{
    return std::abs(position - target);
}

static int64_t step1_cost(const std::vector<int64_t>& positions, int64_t target)
{
    return std::accumulate(positions.begin(), positions.end(), 0,
                           [&](int64_t acc, int64_t elt) { return acc + distance(elt, target); });
}

static int64_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto positions = parse_input(input);
    int64_t median = vector_median(positions);
    return step1_cost(positions, median);
}

static int64_t step2_cost(int64_t position, int64_t target)
{
    int64_t d = distance(position, target);
    return d * (d + 1) / 2;
}

static int64_t step2_cost(const std::vector<int64_t>& positions, int64_t target)
{
    return std::accumulate(positions.begin(), positions.end(), 0,
                           [&](int64_t acc, int64_t elt) { return acc + step2_cost(elt, target); });
}

static int64_t solve_step2(const std::vector<int64_t>& positions, int64_t min, int64_t max)
{
    if (min == max)
    {
        return step2_cost(positions, min);
    }

    int64_t mid1 = min + (max - min) / 2;
    int64_t mid2 = mid1 + 1;
    int64_t mid1_cost = step2_cost(positions, mid1);
    int64_t mid2_cost = step2_cost(positions, mid2);

    if (mid1_cost < mid2_cost)
    {
        return solve_step2(positions, min, mid1);
    }
    else
    {
        return solve_step2(positions, mid2, max);
    }
}

static int64_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto positions = parse_input(input);
    auto [min, max] = std::minmax_element(positions.begin(), positions.end());
    return solve_step2(positions, *min, *max);
}

void Day7::run()
{
    std::cout << "Day7:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;

    auto t0 = std::chrono::steady_clock::now();
    auto s1 = step1("../input/day7");
    auto t1 = std::chrono::steady_clock::now();
    auto s2 = step2("../input/day7");
    auto t2 = std::chrono::steady_clock::now();

    auto t_s1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto t_s2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "    step1: " << s1 << " (" << t_s1 << " ms)\n";
    std::cout << "    step2: " << s2 << " (" << t_s2 << " ms)\n\n";
}

bool Day7::sanity_check()
{
    int64_t step1_expected = 37;
    int64_t step2_expected = 168;
    bool ret = true;

    int64_t step1_got = step1("../input/day7_provided");
    if (step1_got != step1_expected)
    {
        fprintf(stderr, "\n    step1 sanity check failed: expected %lld, got %lld\n", step1_expected, step1_got);
        ret = false;
    }

    int64_t step2_got = step2("../input/day7_provided");
    if (step2_got != step2_expected)
    {
        fprintf(stderr, "\n    step2 sanity check failed: expected %lld, got %lld\n", step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
