#include "common.hh"

using Map = std::vector<std::string>;

bool index_is_valid(size_t x, size_t y, const Map& map)
{
    return x >= 0 && x < map[0].size() && y >= 0 && y < map.size();
}

bool is_low_point(size_t x, size_t y, const Map& map)
{
    char val = map[y][x];
    if (index_is_valid(x - 1, y, map) && val >= map[y][x - 1])
    {
        return false;
    }
    if (index_is_valid(x + 1, y, map) && val >= map[y][x + 1])
    {
        return false;
    }
    if (index_is_valid(x, y - 1, map) && val >= map[y - 1][x])
    {
        return false;
    }
    if (index_is_valid(x, y + 1, map) && val >= map[y + 1][x])
    {
        return false;
    }
    return true;
}

static uint32_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto map = input_to_lines(input);

    uint32_t total = 0;
    for (size_t y = 0; y < map.size(); ++y)
    {
        for (size_t x = 0; x < map[0].size(); ++x)
        {
            if (is_low_point(x, y, map))
            {
                total += (map[y][x] - '0') + 1;
            }
        }
    }

    return total;
}

uint32_t fill_basin(size_t x, size_t y, Map& map)
{
    char val = map[y][x];
    if (val == '9')
    {
        return 0;
    }
    map[y][x] = '9';
    uint32_t total = 1;
    if (index_is_valid(x - 1, y, map) && val <= map[y][x - 1])
    {
        total += fill_basin(x - 1, y, map);
    }
    if (index_is_valid(x + 1, y, map) && val <= map[y][x + 1])
    {
        total += fill_basin(x + 1, y, map);
    }
    if (index_is_valid(x, y - 1, map) && val <= map[y - 1][x])
    {
        total += fill_basin(x, y - 1, map);
    }
    if (index_is_valid(x, y + 1, map) && val <= map[y + 1][x])
    {
        total += fill_basin(x, y + 1, map);
    }
    return total;
}

static uint32_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto map = input_to_lines(input);

    std::array<uint32_t, 4> basin_sizes = {0, 0, 0, 0};
    for (size_t y = 0; y < map.size(); ++y)
    {
        for (size_t x = 0; x < map[0].size(); ++x)
        {
            if (is_low_point(x, y, map))
            {
                basin_sizes[0] = fill_basin(x, y, map);
                std::sort(basin_sizes.begin(), basin_sizes.end());
            }
        }
    }
    return basin_sizes[1] * basin_sizes[2] * basin_sizes[3];
}

void Day9::run()
{
    std::cout << "Day9:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;

    auto t0 = std::chrono::steady_clock::now();
    auto s1 = step1("../input/day9");
    auto t1 = std::chrono::steady_clock::now();
    auto s2 = step2("../input/day9");
    auto t2 = std::chrono::steady_clock::now();

    auto t_s1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto t_s2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "    step1: " << s1 << " (" << t_s1 << " ms)\n";
    std::cout << "    step2: " << s2 << " (" << t_s2 << " ms)\n\n";
}

bool Day9::sanity_check()
{
    uint32_t step1_expected = 15;
    uint32_t step2_expected = 1134;
    bool ret = true;

    uint32_t step1_got = step1("../input/day9_provided");
    if (step1_got != step1_expected)
    {
        fprintf(stderr, "\n    step1 sanity check failed: expected %u, got %u\n", step1_expected, step1_got);
        ret = false;
    }

    uint32_t step2_got = step2("../input/day9_provided");
    if (step2_got != step2_expected)
    {
        fprintf(stderr, "\n    step2 sanity check failed: expected %u, got %u\n", step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
