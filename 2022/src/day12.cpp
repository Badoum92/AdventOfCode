#include "common.h"

#include <deque>

const char* provided_paths[] = {"input/day12_provided"};
uint32_t provided_expected1[] = {31};
uint32_t provided_expected2[] = {29};

const char* real_input = "input/day12";

struct Position
{
    uint32_t x;
    uint32_t y;
};

struct Input
{
    std::vector<std::string> grid;
    Position start_pos;
    Position end_pos;
};

Input parse_input(const char* path)
{
    Input input;
    input.grid = input_to_lines(path);
    for (size_t y = 0; y < input.grid.size(); ++y)
    {
        for (size_t x = 0; x < input.grid[0].size(); ++x)
        {
            if (input.grid[y][x] == 'S')
            {
                input.start_pos = {uint32_t(x), uint32_t(y)};
            }
            else if (input.grid[y][x] == 'E')
            {
                input.end_pos = {uint32_t(x), uint32_t(y)};
            }
        }
    }
    return input;
}

char elevation(const std::vector<std::string>& grid, const Position& position)
{
    char c = grid[position.y][position.x];
    switch (c)
    {
    case 'S':
        return 'a';
    case 'E':
        return 'z';
    }
    return c;
}

bool is_valid(const std::vector<std::string>& grid, const Position& position)
{
    return position.x >= 0 && position.x < uint32_t(grid[0].size()) && position.y >= 0
        && position.y < uint32_t(grid.size());
}

template <typename ELEVATION_TEST>
uint32_t search(const Input& input, const Position& start, char end, ELEVATION_TEST elevation_test)
{
    std::vector<std::vector<uint32_t>> distances(input.grid.size());
    for (auto& d : distances)
    {
        d.resize(input.grid[0].size(), UINT32_MAX);
    }

    distances[start.y][start.x] = 0;
    std::deque<Position> q;
    q.push_back(start);

    while (!q.empty())
    {
        Position pos = q.front();
        q.pop_front();
        uint32_t d = distances[pos.y][pos.x];

        if (input.grid[pos.y][pos.x] == end)
        {
            return d;
        }

        Position positions[] = {{pos.x - 1, pos.y}, {pos.x + 1, pos.y}, {pos.x, pos.y - 1}, {pos.x, pos.y + 1}};
        for (const auto& next : positions)
        {
            if (is_valid(input.grid, next) && elevation_test(input, pos, next) && distances[next.y][next.x] > d + 1)
            {
                distances[next.y][next.x] = d + 1;
                q.push_back(next);
            }
        }
    }
    return 0;
}

bool elevation_test1(const Input& input, const Position& pos, const Position& next)
{
    return elevation(input.grid, next) <= elevation(input.grid, pos) + 1;
}

uint32_t step1(const Input& input)
{
    return search(input, input.start_pos, 'E', elevation_test1);
}

bool elevation_test2(const Input& input, const Position& pos, const Position& next)
{
    return elevation(input.grid, next) >= elevation(input.grid, pos) - 1;
}

uint32_t step2(const Input& input)
{
    return search(input, input.end_pos, 'a', elevation_test2);
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
