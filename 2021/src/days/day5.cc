#include "common.hh"

struct Point
{
    int32_t x = 0;
    int32_t y = 0;

    bool operator==(const Point& other) const = default;

    Point& operator+=(const Point& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
};

namespace std
{
    template <>
    struct hash<Point>
    {
        size_t operator()(const Point& point) const
        {
            return std::hash<int64_t>()(*(int64_t*)&point);
        }
    };
} // namespace std

struct Line
{
    Point a;
    Point b;

    bool is_diagonal() const
    {
        return a.x != b.x && a.y != b.y;
    }

    Point iterator() const
    {
        Point it;
        if (a.x != b.x)
        {
            it.x = a.x < b.x ? 1 : -1;
        }
        if (a.y != b.y)
        {
            it.y = a.y < b.y ? 1 : -1;
        }
        return it;
    }

    size_t size() const
    {
        return std::max(std::abs(b.x - a.x), std::abs(b.y - a.y));
    }
};

std::vector<Line> parse_input(std::ifstream& input)
{
    auto lines_str = input_to_lines(input);
    std::vector<Line> lines(lines_str.size());
    for (size_t i = 0; i < lines_str.size(); ++i)
    {
        auto& line = lines[i];
        const auto& line_str = lines_str[i];

        auto points_str = split(line_str, " -> ");
        auto point_a_str = split(points_str[0], ",");
        auto point_b_str = split(points_str[1], ",");

        line.a.x = std::stoul(point_a_str[0]);
        line.a.y = std::stoul(point_a_str[1]);
        line.b.x = std::stoul(point_b_str[0]);
        line.b.y = std::stoul(point_b_str[1]);
    }
    return lines;
}

std::unordered_set<Point> intersections(const std::vector<Line>& lines, bool diagonals)
{
    std::unordered_set<Point> visited;
    std::unordered_set<Point> intersected;

    for (const auto& line : lines)
    {
        if (!diagonals && line.is_diagonal())
        {
            continue;
        }

        Point point = line.a;
        auto iter = line.iterator();
        for (size_t i = 0; i <= line.size(); ++i, point += iter)
        {
            if (!visited.insert(point).second)
            {
                intersected.insert(point);
            }
        }
    }

    return intersected;
}

static uint32_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = parse_input(input);
    return intersections(lines, false).size();
}

static uint32_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = parse_input(input);
    return intersections(lines, true).size();
}

void Day5::run()
{
    std::cout << "Day5:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;

    auto t0 = std::chrono::steady_clock::now();
    auto s1 = step1("../input/day5");
    auto t1 = std::chrono::steady_clock::now();
    auto s2 = step2("../input/day5");
    auto t2 = std::chrono::steady_clock::now();

    auto t_s1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto t_s2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "    step1: " << s1 << " (" << t_s1 << " ms)\n";
    std::cout << "    step2: " << s2 << " (" << t_s2 << " ms)\n\n";
}

bool Day5::sanity_check()
{
    uint32_t step1_expected = 5;
    uint32_t step2_expected = 12;
    bool ret = true;

    uint32_t step1_got = step1("../input/day5_provided");
    if (step1_got != step1_expected)
    {
        fprintf(stderr, "\n    step1 sanity check failed: expected %u, got %u\n", step1_expected, step1_got);
        ret = false;
    }

    uint32_t step2_got = step2("../input/day5_provided");
    if (step2_got != step2_expected)
    {
        fprintf(stderr, "\n    step2 sanity check failed: expected %u, got %u\n", step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
