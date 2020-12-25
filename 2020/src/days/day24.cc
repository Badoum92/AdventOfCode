#include "common.hh"

#include <map>

#define WHITE false
#define BLACK true

using Coord = std::pair<int, int>;
using Grid = std::map<Coord, bool>;

static const Coord NE(0, 1);
static const Coord E(1, 0);
static const Coord SE(1, -1);
static const Coord SW(0, -1);
static const Coord W(-1, 0);
static const Coord NW(-1, 1);

std::array<Coord, 6> directions = {NE, E, SE, SW, W, NW};

static Coord operator+(const Coord& a, const Coord& b)
{
    return std::make_pair(a.first + b.first, a.second + b.second);
}

static Coord& operator+=(Coord& a, const Coord& b)
{
    a.first += b.first;
    a.second += b.second;
    return a;
}

static const Coord& dir_from_string(const std::string& s)
{
    if (s == "ne")
        return NE;
    else if (s == "e")
        return E;
    else if (s == "se")
        return SE;
    else if (s == "sw")
        return SW;
    else if (s == "w")
        return W;
    else if (s == "nw")
        return NW;
    assert(false);
    return NE;
}

static void parse_str(const std::string& str, Grid& grid)
{
    Coord coord = std::make_pair(0, 0);
    std::string tmp;
    for (unsigned i = 0; i < str.size();)
    {
        tmp += str[i++];
        if (tmp[0] == 's' || tmp[0] == 'n')
            tmp += str[i++];
        coord += dir_from_string(tmp);
        tmp.clear();
    }
    grid[coord] = !grid[coord];
}

static unsigned step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);
    Grid grid;
    for (const auto& s : lines)
    {
        parse_str(s, grid);
    }
    return std::count_if(begin(grid), end(grid),
                         [&](const auto& x) { return x.second == BLACK; });
}

static unsigned count_black_neighbours(Grid& grid, const Coord& coord)
{
    unsigned count = 0;
    for (const auto& dir : directions)
    {
        if (grid[coord + dir] == BLACK)
        {
            ++count;
        }
    }
    return count;
}

static void flip_tiles(Grid& cur, Grid& prev)
{
    Grid cpy = prev;
    for (const auto& [coord, color] : prev)
    {
        unsigned n = count_black_neighbours(cpy, coord);
        if (color == BLACK)
        {
            cur[coord] = n == 0 || n > 2 ? WHITE : BLACK;
        }
        else
        {
            cur[coord] = n == 2 ? BLACK : WHITE;
        }
    }
    std::swap(prev, cpy);
}

static void expand(Grid& grid)
{
    Grid cpy = grid;
    for (const auto& [coord, color] : cpy)
    {
        if (color == WHITE)
            continue;
        for (const auto& dir : directions)
        {
            if (!grid.count(coord + dir))
                grid[coord + dir] = WHITE;
        }
    }
}

static unsigned step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);
    Grid grid1;
    for (const auto& s : lines)
    {
        parse_str(s, grid1);
    }
    Grid grid2 = grid1;
    for (unsigned i = 0; i < 100; ++i)
    {
        std::swap(grid1, grid2);
        expand(grid2);
        flip_tiles(grid1, grid2);
    }
    return std::count_if(begin(grid1), end(grid1),
                         [&](const auto& x) { return x.second == BLACK; });
}

void Day24::run()
{
    std::cout << "Day24:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day24::sanity_check()
{
    unsigned step1_expected = 10;
    unsigned step2_expected = 2208;
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
