#include "common.hh"

using Grid = std::vector<std::vector<std::string>>;

static bool valid(int x, int y, int z, const Grid& grid)
{
    return x >= 0 && y >= 0 && z >= 0 && z < (int)grid.size()
        && y < (int)grid[z].size() && x < (int)grid[z][y].size();
}

static int count_neighbours(int x, int y, int z, const Grid& grid)
{
    int count = 0;
    for (int z_inc = -1; z_inc <= 1; ++z_inc)
    {
        for (int y_inc = -1; y_inc <= 1; ++y_inc)
        {
            for (int x_inc = -1; x_inc <= 1; ++x_inc)
            {
                if (z_inc == 0 && y_inc == 0 && x_inc == 0)
                    continue;

                if (!valid(x + x_inc, y + y_inc, z + z_inc, grid))
                    continue;

                if (grid[z + z_inc][y + y_inc][x + x_inc] == '#')
                {
                    ++count;
                    if (z == 0 && z_inc != 0)
                        ++count;
                }
            }
        }
    }
    return count;
}

static void update_grid(Grid& cur, const Grid& prev, unsigned iter)
{
    for (unsigned z = 0; z < cur.size() - (5 - iter); ++z)
    {
        for (unsigned y = (5 - iter); y < cur[z].size() - (5 - iter); ++y)
        {
            for (unsigned x = (5 - iter); x < cur[z][y].size() - (5 - iter); ++x)
            {
                int n = count_neighbours(x, y, z, prev);
                char c = prev[z][y][x];

                if (c == '#')
                {
                    if (n == 2 || n == 3)
                        cur[z][y][x] = '#';
                    else
                        cur[z][y][x] = '.';
                }
                else if (c == '.')
                {
                    if (n != 3)
                        cur[z][y][x] = '.';
                    else
                        cur[z][y][x] = '#';
                }
            }
        }
    }
}

static unsigned count_active(const Grid& grid)
{
    unsigned count = 0;
    for (unsigned z = 0; z < grid.size(); ++z)
    {
        for (unsigned y = 0; y < grid[z].size(); ++y)
        {
            for (unsigned x = 0; x < grid[z][y].size(); ++x)
            {
                if (grid[z][y][x] == '#')
                {
                    ++count;
                    if (z != 0)
                        ++count;
                }
            }
        }
    }
    return count;
}

static Grid init_empty_grid(size_t size)
{
    Grid grid;
    grid.resize(size / 2);
    for (unsigned z = 0; z < grid.size(); ++z)
    {
        grid[z].resize(size);
        for (unsigned y = 0; y < grid[z].size(); ++y)
        {
            grid[z][y].resize(size);
            for (unsigned x = 0; x < grid[z][y].size(); ++x)
            {
                grid[z][y][x] = '.';
            }
        }
    }
    return grid;
}

static unsigned step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);
    size_t size = lines.size() + 12;
    for (unsigned i = 0; i < lines.size(); ++i)
    {
        lines[i] = "......" + lines[i] + "......";
    }
    for (unsigned i = 0; i < 6; ++i)
    {
        lines.push_back(std::string(size, '.'));
        lines.insert(begin(lines), std::string(size, '.'));
    }

    Grid grid1 = init_empty_grid(size);
    grid1[0] = lines;
    Grid grid2 = grid1;

    for (unsigned i = 0; i < 6; ++i)
    {
        update_grid(grid1, grid2, i);
        std::swap(grid1, grid2);
    }
    return count_active(grid2);
}

using HGrid = std::vector<std::vector<std::vector<std::string>>>;

static bool valid2(int x, int y, int z, int w, const HGrid& grid)
{
    return x >= 0 && y >= 0 && z >= 0 && w >= 0 && w < (int)grid.size()
        && z < (int)grid[w].size() && y < (int)grid[w][z].size()
        && x < (int)grid[w][z][y].size();
}

static int count_neighbours2(int x, int y, int z, int w, const HGrid& grid)
{
    int count = 0;
    for (int w_inc = -1; w_inc <= 1; ++w_inc)
    {
        for (int z_inc = -1; z_inc <= 1; ++z_inc)
        {
            for (int y_inc = -1; y_inc <= 1; ++y_inc)
            {
                for (int x_inc = -1; x_inc <= 1; ++x_inc)
                {
                    if (w_inc == 0 && z_inc == 0 && y_inc == 0 && x_inc == 0)
                        continue;

                    if (!valid2(x + x_inc, y + y_inc, z + z_inc, w + w_inc,
                                grid))
                        continue;

                    if (grid[w + w_inc][z + z_inc][y + y_inc][x + x_inc] == '#')
                    {
                        unsigned tmp = 1;
                        if (z == 0 && z_inc != 0)
                        {
                            tmp *= 2;
                        }
                        if (w == 0 && w_inc != 0)
                        {
                            tmp *= 2;
                        }
                        count += tmp;
                    }
                }
            }
        }
    }
    return count;
}

static void update_grid2(HGrid& cur, const HGrid& prev, unsigned iter)
{
    for (unsigned w = 0; w < cur.size() - (5 - iter); ++w)
    {
        for (unsigned z = 0; z < cur[w].size() - (5 - iter); ++z)
        {
            for (unsigned y = (5 - iter); y < cur[w][z].size() - (5 - iter); ++y)
            {
                for (unsigned x = (5 - iter); x < cur[w][z][y].size() - (5 - iter); ++x)
                {
                    int n = count_neighbours2(x, y, z, w, prev);
                    char c = prev[w][z][y][x];

                    if (c == '#')
                    {
                        if (n == 2 || n == 3)
                            cur[w][z][y][x] = '#';
                        else
                            cur[w][z][y][x] = '.';
                    }
                    else if (c == '.')
                    {
                        if (n != 3)
                            cur[w][z][y][x] = '.';
                        else
                            cur[w][z][y][x] = '#';
                    }
                }
            }
        }
    }
}

static unsigned count_active2(const HGrid& grid)
{
    unsigned count = 0;
    for (unsigned w = 0; w < grid.size(); ++w)
    {
        for (unsigned z = 0; z < grid[w].size(); ++z)
        {
            for (unsigned y = 0; y < grid[w][z].size(); ++y)
            {
                for (unsigned x = 0; x < grid[w][z][y].size(); ++x)
                {
                    if (grid[w][z][y][x] == '#')
                    {
                        unsigned tmp = 1;
                        if (z != 0)
                        {
                            tmp *= 2;
                        }
                        if (w != 0)
                        {
                            tmp *= 2;
                        }
                        count += tmp;
                    }
                }
            }
        }
    }
    return count;
}

static HGrid init_empty_grid2(size_t size)
{
    HGrid grid;
    grid.resize(size / 2);
    for (unsigned w = 0; w < grid.size(); ++w)
    {
        grid[w].resize(size / 2);
        for (unsigned z = 0; z < grid[w].size(); ++z)
        {
            grid[w][z].resize(size);
            for (unsigned y = 0; y < grid[w][z].size(); ++y)
            {
                grid[w][z][y].resize(size);
                for (unsigned x = 0; x < grid[w][z][y].size(); ++x)
                {
                    grid[w][z][y][x] = '.';
                }
            }
        }
    }
    return grid;
}

static unsigned step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);
    size_t size = lines.size() + 12;
    for (unsigned i = 0; i < lines.size(); ++i)
    {
        lines[i] = "......" + lines[i] + "......";
    }
    for (unsigned i = 0; i < 6; ++i)
    {
        lines.push_back(std::string(size, '.'));
        lines.insert(begin(lines), std::string(size, '.'));
    }

    HGrid grid1 = init_empty_grid2(size);
    grid1[0][0] = lines;
    HGrid grid2 = grid1;

    for (unsigned i = 0; i < 6; ++i)
    {
        update_grid2(grid1, grid2, i);
        std::swap(grid1, grid2);
    }
    return count_active2(grid2);
}

void Day17::run()
{
    std::cout << "Day17:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day17::sanity_check()
{
    unsigned step1_expected = 112;
    unsigned step2_expected = 848;
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
