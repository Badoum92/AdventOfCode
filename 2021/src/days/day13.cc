#include "common.hh"

struct Point
{
    uint32_t x;
    uint32_t y;

    bool operator==(const Point& other) const = default;

    Point fold_x(uint32_t index) const
    {
        assert(x != index);
        uint32_t new_x = index > x ? x : 2 * index - x;
        return {new_x, y};
    }

    Point fold_y(uint32_t index) const
    {
        assert(y != index);
        uint32_t new_y = index > y ? y : 2 * index - y;
        return {x, new_y};
    }
};

namespace std
{
    template <>
    struct hash<Point>
    {
        size_t operator()(const Point& point) const
        {
            return std::hash<int64_t>()(*(uint64_t*)&point);
        }
    };
} // namespace std

struct Fold
{
    char axis;
    uint32_t index;
};

std::pair<std::vector<Point>, std::vector<Fold>> parse_input(std::ifstream& input)
{
    std::pair<std::vector<Point>, std::vector<Fold>> ret;
    auto& points = ret.first;
    auto& folds = ret.second;

    auto lines = input_to_lines(input);

    points.reserve(lines.size());
    folds.reserve(16);

    size_t i = 0;
    for (; lines[i] != ""; ++i)
    {
        auto s = split(lines[i], ",");
        points.push_back({std::stoul(s[0]), std::stoul(s[1])});
    }

    ++i;
    for (; i < lines.size(); ++i)
    {
        auto s = split(lines[i], "=");
        folds.push_back({s[0][s[0].size() - 1], std::stoul(s[1])});
    }

    return ret;
}

static std::vector<Point> fold_x(const std::vector<Point>& points, uint32_t index)
{
    std::vector<Point> ret;
    ret.reserve(points.size());

    for (const auto& p : points)
    {
        if (p.x == index)
        {
            continue;
        }
        ret.push_back(p.fold_x(index));
    }
    return ret;
}

static std::vector<Point> fold_y(const std::vector<Point>& points, uint32_t index)
{
    std::vector<Point> ret;
    ret.reserve(points.size());
    for (const auto& p : points)
    {
        if (p.y == index)
        {
            continue;
        }
        ret.push_back(p.fold_y(index));
    }
    return ret;
}

static std::vector<Point> fold(const std::vector<Point>& points, const Fold& fold)
{
    if (fold.axis == 'x')
    {
        return fold_x(points, fold.index);
    }
    else
    {
        return fold_y(points, fold.index);
    }
}

static uint32_t count_points(const std::vector<Point>& points)
{
    std::unordered_set<Point> set;
    set.reserve(points.size());
    for (const auto& p : points)
    {
        set.insert(p);
    }
    return set.size();
}

static uint32_t step1(const char* input_file)
{
    std::ifstream input(input_file);

    auto [points, folds] = parse_input(input);

    return count_points(fold(points, folds[0]));
}

static uint32_t step2(const char* input_file)
{
    std::ifstream input(input_file);

    auto [points, folds] = parse_input(input);

    for (const auto& f : folds)
    {
        points = fold(points, f);
    }

    constexpr size_t size_x = 40;
    constexpr size_t size_y = 6;
    std::array<bool, size_x * size_y> grid;
    grid.fill(false);

    for (const auto& p : points)
    {
        grid[p.y * size_x + p.x] = true;
    }

    for (size_t y = 0; y < size_y; ++y)
    {
        for (size_t x = 0; x < size_x; ++x)
        {
            std::cout << (grid[y * size_x + x] ? '#' : ' ');
        }
        std::cout << "\n";
    }

    return 0;
}

void Day13::run()
{
    std::cout << "Day13:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;

    auto t0 = std::chrono::steady_clock::now();
    auto s1 = step1("../input/day13");
    auto t1 = std::chrono::steady_clock::now();
    auto s2 = step2("../input/day13");
    auto t2 = std::chrono::steady_clock::now();

    auto t_s1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto t_s2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "    step1: " << s1 << " (" << t_s1 << " ms)\n";
    std::cout << "    step2: " << s2 << " (" << t_s2 << " ms)\n\n";
}

bool Day13::sanity_check()
{
    uint32_t step1_expected = 17;
    uint32_t step2_expected = 0;
    bool ret = true;

    uint32_t step1_got = step1("../input/day13_provided");
    if (step1_got != step1_expected)
    {
        fprintf(stderr, "\n    step1 sanity check failed: expected %u, got %u\n", step1_expected, step1_got);
        ret = false;
    }

    uint32_t step2_got = step2("../input/day13_provided");
    if (step2_got != step2_expected)
    {
        fprintf(stderr, "\n    step2 sanity check failed: expected %u, got %u\n", step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
