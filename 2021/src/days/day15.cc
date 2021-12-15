#include "common.hh"

template <typename T>
using Grid = std::vector<std::vector<T>>;
using Point = std::pair<size_t, size_t>;

namespace std
{
    template <>
    struct hash<Point>
    {
        size_t operator()(const Point& point) const
        {
            return std::hash<size_t>()(point.first) ^ std::hash<size_t>()(point.second);
        }
    };
} // namespace std

static Grid<size_t> parse_input1(std::ifstream& input)
{
    auto lines = input_to_lines(input);
    Grid<size_t> ret(lines.size());
    for (size_t i = 0; i < lines.size(); ++i)
    {
        ret[i].resize(lines[i].size());
        for (size_t j = 0; j < lines[i].size(); ++j)
        {
            ret[i][j] = lines[i][j] - '0';
        }
    }
    return ret;
}

static bool valid_index(const Point& p, const Grid<size_t>& grid)
{
    return p.second < grid.size() && p.first < grid[0].size();
}

static size_t dijkstra(const Grid<size_t>& grid, const Point& source)
{
    std::multimap<size_t, Point> map;
    std::unordered_set<Point> seen;

    map.emplace(0, source);

    while (!map.empty())
    {
        auto [dist, p] = *map.begin();
        map.erase(map.begin());

        if (p.first == grid[0].size() - 1 && p.second == grid.size() - 1)
        {
            return dist;
        }

        if (!seen.insert(p).second)
        {
            continue;
        }

        std::array<Point, 4> neighbours = {Point{p.first + 1, p.second}, Point{p.first - 1, p.second},
                                           Point{p.first, p.second + 1}, Point{p.first, p.second - 1}};

        for (const auto& n : neighbours)
        {
            if (!valid_index(n, grid))
            {
                continue;
            }
            map.emplace(dist + grid[n.second][n.first], n);
        }
    }
    return 0;
}

static size_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto grid = parse_input1(input);
    return dijkstra(grid, Point{0, 0});
}

static Grid<size_t> parse_input2(std::ifstream& input)
{
    auto grid = parse_input1(input);
    Grid<size_t> ret(grid.size() * 5);
    for (size_t i = 0; i < grid.size() * 5; ++i)
    {
        ret[i].resize(grid.size() * 5);
    }
    for (size_t yy = 0; yy < 5; ++yy)
    {
        for (size_t xx = 0; xx < 5; ++xx)
        {
            for (size_t y = 0; y < grid.size(); ++y)
            {
                for (size_t x = 0; x < grid[y].size(); ++x)
                {
                    size_t val = grid[y][x] + xx + yy;
                    if (val > 9)
                    {
                        val -= 9;
                    }
                    ret[yy * grid.size() + y][xx * grid.size() + x] = val;
                }
            }
        }
    }
    return ret;
}

static size_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto grid = parse_input2(input);
    return dijkstra(grid, Point{0, 0});
}

void Day15::run()
{
    std::cout << "Day15:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;

    auto t0 = std::chrono::steady_clock::now();
    auto s1 = step1("../input/day15");
    auto t1 = std::chrono::steady_clock::now();
    auto s2 = step2("../input/day15");
    auto t2 = std::chrono::steady_clock::now();

    auto t_s1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto t_s2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "    step1: " << s1 << " (" << t_s1 << " ms)\n";
    std::cout << "    step2: " << s2 << " (" << t_s2 << " ms)\n\n";
}

bool Day15::sanity_check()
{
    size_t step1_expected = 40;
    size_t step2_expected = 315;
    bool ret = true;

    size_t step1_got = step1("../input/day15_provided");
    if (step1_got != step1_expected)
    {
        fprintf(stderr, "\n    step1 sanity check failed: expected %zu, got %zu\n", step1_expected, step1_got);
        ret = false;
    }

    size_t step2_got = step2("../input/day15_provided");
    if (step2_got != step2_expected)
    {
        fprintf(stderr, "\n    step2 sanity check failed: expected %zu, got %zu\n", step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
