#include "common.hh"

using Grid = std::vector<std::string>;

static bool index_is_valid(size_t x, size_t y)
{
    return x < 10 && y < 10;
}

template <typename F>
static void for_each_octopus(Grid& grid, const F& func)
{
    for (size_t y = 0; y < 10; ++y)
    {
        for (size_t x = 0; x < 10; ++x)
        {
            func(x, y, grid[y][x]);
        }
    }
}

template <typename F>
static void for_each_neighbour(size_t x, size_t y, Grid& grid, const F& func)
{
    for (int yy = -1; yy <= 1; ++yy)
    {
        for (int xx = -1; xx <= 1; ++xx)
        {
            if ((xx == 0 && yy == 0) || !index_is_valid(x + xx, y + yy))
            {
                continue;
            }
            func(grid[y + yy][x + xx]);
        }
    }
}

static uint32_t step_day(Grid& grid)
{
    uint32_t flashes = 0;
    for_each_octopus(grid, [&](size_t, size_t, char& c) { ++c; });
    bool found_9 = false;
    do
    {
        found_9 = false;
        for_each_octopus(grid, [&](size_t x, size_t y, char& c) {
            if (c > '9' && c < 120)
            {
                flashes++;
                c = 120;
                found_9 = true;

                for_each_neighbour(x, y, grid, [&](char& c) {
                    c += c == 120 ? 0 : 1;
                });
            }
        });
    } while (found_9);

    for_each_octopus(grid, [&](size_t, size_t, char& c) {
        c = c > '9' ? '0' : c;
    });
    return flashes;
}

static uint32_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto grid = input_to_lines(input);

    uint32_t flashes = 0;
    for (size_t i = 0; i < 100; ++i)
    {
        flashes += step_day(grid);
    }

    return flashes;
}

static uint32_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto grid = input_to_lines(input);

    for (uint32_t day = 1;; ++day)
    {
        if (step_day(grid) == 100)
        {
            return day;
        }
    }
}

void Day11::run()
{
    std::cout << "Day11:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;

    auto t0 = std::chrono::steady_clock::now();
    auto s1 = step1("../input/day11");
    auto t1 = std::chrono::steady_clock::now();
    auto s2 = step2("../input/day11");
    auto t2 = std::chrono::steady_clock::now();

    auto t_s1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto t_s2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "    step1: " << s1 << " (" << t_s1 << " ms)\n";
    std::cout << "    step2: " << s2 << " (" << t_s2 << " ms)\n\n";
}

bool Day11::sanity_check()
{
    uint32_t step1_expected = 1656;
    uint32_t step2_expected = 195;
    bool ret = true;

    uint32_t step1_got = step1("../input/day11_provided");
    if (step1_got != step1_expected)
    {
        fprintf(stderr, "\n    step1 sanity check failed: expected %u, got %u\n", step1_expected, step1_got);
        ret = false;
    }

    uint32_t step2_got = step2("../input/day11_provided");
    if (step2_got != step2_expected)
    {
        fprintf(stderr, "\n    step2 sanity check failed: expected %u, got %u\n", step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
