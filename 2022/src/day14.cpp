#include "common.h"

const char* provided_paths[] = {"input/day14_provided"};
uint64_t provided_expected1[] = {0};
uint64_t provided_expected2[] = {0};

const char* real_input = "input/day14";

struct Position
{
    uint32_t x;
    uint32_t y;
};

struct Input
{
    std::vector<std::vector<Position>> shapes;
    std::vector<std::vector<char>> grid;
    std::vector<Position> sand;
    Position min = {UINT32_MAX, UINT32_MAX};
    Position max = {0, 0};
};

std::ostream& operator<<(std::ostream& os, const Position& pos)
{
    return os << "(" << pos.x << ", " << pos.y << ")";
}

Input parse_input(const char* path)
{
    Input input;
    auto lines = input_to_lines(path);
    input.shapes.reserve(lines.size());
    for (const auto& line : lines)
    {
        auto& points = input.shapes.emplace_back();
        auto line_split = split(line, " -> ");
        for (const auto& coord : line_split)
        {
            Position& pos = points.emplace_back();
            sscanf(coord.c_str(), "%u,%u", &pos.x, &pos.y);
            input.min.x = std::min(input.min.x, pos.x);
            input.min.y = std::min(input.min.y, pos.y);
            input.max.x = std::max(input.max.x, pos.x);
            input.max.y = std::max(input.max.y, pos.y);
        }
    }

    input.grid.resize(input.max.y + 1);
    for (auto& row : input.grid)
    {
        row.resize(input.max.x + 1, '.');
    }

    for (const auto& shape : input.shapes)
    {
        for (size_t i = 0; i < shape.size() - 1; ++i)
        {
            const auto& begin = shape[i];
            const auto& end = shape[i + 1];
            for (size_t y = std::min(begin.y, end.y); y <= std::max(begin.y, end.y); ++y)
            {
                for (size_t x = std::min(begin.x, end.x); x <= std::max(begin.x, end.x); ++x)
                {
                    input.grid[y][x] = '#';
                }
            }
        }
    }

    /* for (size_t y = 0; y < input.grid.size(); ++y)
    {
        for (size_t x = 0; x < input.grid[0].size(); ++x)
        {
            std::cout << input.grid[y][x];
        }
        std::cout << "\n";
    }

    std::cout << "min: " << input.min << "\n";
    std::cout << "max: " << input.max << "\n"; */

    return input;
}

bool simulate(Input& input)
{

}

uint64_t step1(Input input)
{
    return 0;
}

uint64_t step2(const Input& input)
{
    return 0;
}

int main()
{
    for (size_t i = 0; i < sizeof(provided_paths) / sizeof(provided_paths[0]); ++i)
    {
        Input input = parse_input(provided_paths[i]);
        auto [res1, time1] = time_function(step1, input);
        auto [res2, time2] = time_function(step2, input);

        std::cout << "Provided input #" << i << "\n";
        std::cout << "    Step1: Expected: " << provided_expected1[i] << "  |  Result: " << res1
                  << "  |  Time: " << time1 << "  |  " << (res1 == provided_expected1[i] ? "OK" : "KO") << "\n";
        std::cout << "    Step2: Expected: " << provided_expected2[i] << "  |  Result: " << res2
                  << "  |  Time: " << time2 << "  |  " << (res2 == provided_expected2[i] ? "OK" : "KO") << "\n";

        if (res1 != provided_expected1[i] || res2 != provided_expected2[i])
        {
            return 1;
        }
    }

    Input input = parse_input(real_input);
    auto [res1, time1] = time_function(step1, input);
    auto [res2, time2] = time_function(step2, input);
    std::cout << "\n=================================\n";
    std::cout << "Step1: Result: " << res1 << "  |  Time: " << time1 << " ms\n";
    std::cout << "Step2: Result: " << res2 << "  |  Time: " << time2 << " ms\n";
}
