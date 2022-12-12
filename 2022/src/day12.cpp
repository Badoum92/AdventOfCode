#include "common.h"

const char* provided_paths[] = {"input/day12_provided"};
int64_t provided_expected1[] = {31};
int64_t provided_expected2[] = {0};

const char* real_input = "input/day12";

struct Position
{
    int64_t x;
    int64_t y;
};

struct Input
{
    std::vector<std::string> grid;
    Position pos;
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
                input.pos = {int64_t(x), int64_t(y)};
                return input;
            }
        }
    }
    return input;
}

int64_t move(const std::vector<std::string>& grid, std::vector<std::vector<bool>>& visited, const Position& position)
{

}

int64_t step1(const Input& input)
{
    std::vector<std::vector<bool>> visited(input.grid.size());
    for (auto& v : visited)
    {
        v.resize(input.grid[0].size());
    }
    visited[input.pos.y][input.pos.x] = true;
}

int64_t step2(const Input& input)
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
