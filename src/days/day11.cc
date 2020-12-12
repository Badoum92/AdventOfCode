#include "common.hh"

static bool in_grid(unsigned x, unsigned y, const std::vector<std::string>& v)
{
    return y < v.size() && x < v[0].size();
}

static unsigned count_neighbours(unsigned x, unsigned y,
                                 const std::vector<std::string>& v)
{
    unsigned total = 0;
    for (int Y = -1; Y <= 1; ++Y)
    {
        for (int X = -1; X <= 1; ++X)
        {
            if ((X == 0 && Y == 0) || !in_grid(x + X, y + Y, v))
                continue;
            if (v[y + Y][x + X] == '#')
                ++total;
        }
    }
    return total;
}

static char get_new_char(unsigned x, unsigned y,
                         const std::vector<std::string>& v)
{
    char cur = v[y][x];
    if (cur == '.')
        return cur;

    unsigned neighbours = count_neighbours(x, y, v);
    if (cur == 'L' && neighbours == 0)
    {
        return '#';
    }
    if (cur == '#' && neighbours >= 4)
    {
        return 'L';
    }
    return cur;
}

static unsigned step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto cur = input_to_lines(input);
    auto prev = cur;

    while (true)
    {
        bool changed = false;
        for (unsigned y = 0; y < cur.size(); ++y)
        {
            for (unsigned x = 0; x < cur[0].size(); ++x)
            {
                char new_char = get_new_char(x, y, prev);
                if (new_char != cur[y][x])
                    changed = true;
                cur[y][x] = new_char;
            }
        }
        if (!changed)
            break;
        std::swap(cur, prev);
    }

    return std::accumulate(begin(cur), end(cur), 0,
                           [&](int x, const std::string& s) {
                               return x + std::count(begin(s), end(s), '#');
                           });
}

static char find_in_direction(unsigned x, unsigned y, int dir_x, int dir_y,
                              const std::vector<std::string>& v)
{
    while (in_grid(x + dir_x, y + dir_y, v))
    {
        char cur = v[y + dir_y][x + dir_x];
        if (cur != '.')
            return cur;
        x += dir_x;
        y += dir_y;
    }
    return '.';
}

static unsigned count_neighbours2(unsigned x, unsigned y,
                                  const std::vector<std::string>& v)
{
    unsigned total = 0;
    total += find_in_direction(x, y, 1, 0, v) == '#' ? 1 : 0;
    total += find_in_direction(x, y, 1, 1, v) == '#' ? 1 : 0;
    total += find_in_direction(x, y, 0, 1, v) == '#' ? 1 : 0;
    total += find_in_direction(x, y, -1, 1, v) == '#' ? 1 : 0;
    total += find_in_direction(x, y, -1, 0, v) == '#' ? 1 : 0;
    total += find_in_direction(x, y, -1, -1, v) == '#' ? 1 : 0;
    total += find_in_direction(x, y, 0, -1, v) == '#' ? 1 : 0;
    total += find_in_direction(x, y, 1, -1, v) == '#' ? 1 : 0;
    return total;
}

static char get_new_char2(unsigned x, unsigned y,
                          const std::vector<std::string>& v)
{
    char cur = v[y][x];
    if (cur == '.')
        return cur;

    unsigned neighbours = count_neighbours2(x, y, v);
    if (cur == 'L' && neighbours == 0)
    {
        return '#';
    }
    if (cur == '#' && neighbours >= 5)
    {
        return 'L';
    }
    return cur;
}

static unsigned step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto cur = input_to_lines(input);
    auto prev = cur;

    while (true)
    {
        bool changed = false;
        for (unsigned y = 0; y < cur.size(); ++y)
        {
            for (unsigned x = 0; x < cur[0].size(); ++x)
            {
                char new_char = get_new_char2(x, y, prev);
                if (new_char != cur[y][x])
                    changed = true;
                cur[y][x] = new_char;
            }
        }
        if (!changed)
            break;
        std::swap(cur, prev);
    }

    return std::accumulate(begin(cur), end(cur), 0,
                           [&](int x, const std::string& s) {
                               return x + std::count(begin(s), end(s), '#');
                           });
}

void Day11::run()
{
    std::cout << "Day11:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;

    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day11::sanity_check()
{
    unsigned step1_expected = 37;
    unsigned step2_expected = 26;
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
