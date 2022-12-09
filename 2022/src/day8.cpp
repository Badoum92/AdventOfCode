#include "common.h"

const char* provided_paths[] = {"input/day8_provided"};
uint64_t provided_expected1[] = {21};
uint64_t provided_expected2[] = {8};

const char* real_input = "input/day8";

using Input = std::vector<std::string>;

Input parse_input(const char* path)
{
    return input_to_lines(path);
}

void check_row(const Input& input, size_t row, std::vector<std::vector<bool>>& visible)
{
    size_t size = input[0].size();
    visible[row][0] = true;
    visible[row][size - 1] = true;
    char max_left = input[row][0];
    char max_right = input[row][size - 1];
    for (size_t i = 1; i < size; ++i)
    {
        if (input[row][i] > max_left)
        {
            visible[row][i] = true;
            max_left = input[row][i];
        }
        if (input[row][size - 1 - i] > max_right)
        {
            visible[row][size - 1 - i] = true;
            max_right = input[row][size - 1 - i];
        }
    }
}

void check_col(const Input& input, size_t col, std::vector<std::vector<bool>>& visible)
{
    size_t size = input.size();
    visible[0][col] = true;
    visible[size - 1][col] = true;
    char max_up = input[0][col];
    char max_down = input[size - 1][col];
    for (size_t i = 1; i < size; ++i)
    {
        if (input[i][col] > max_up)
        {
            visible[i][col] = true;
            max_up = input[i][col];
        }
        if (input[size - 1 - i][col] > max_down)
        {
            visible[size - 1 - i][col] = true;
            max_down = input[size - 1 - i][col];
        }
    }
}

uint64_t step1(const Input& input)
{
    std::vector<std::vector<bool>> visible(input.size());
    for (auto& v : visible)
    {
        v.resize(input[0].size(), false);
    }

    for (size_t i = 0; i < input.size(); ++i)
    {
        check_row(input, i, visible);
    }
    for (size_t i = 0; i < input[0].size(); ++i)
    {
        check_col(input, i, visible);
    }

    uint64_t count = 0;
    for (const auto& v : visible)
    {
        for (const auto& b : v)
        {
            count += b;
        }
    }
    return count;
}

uint64_t lines_of_sight(const Input& input, int x, int y)
{
    uint64_t left = 0;
    uint64_t right = 0;
    uint64_t up = 0;
    uint64_t down = 0;
    char val = input[y][x];
    for (int xx = x - 1; xx >= 0; --xx)
    {
        left++;
        if (input[y][xx] >= val)
        {
            break;
        }
    }
    for (int xx = x + 1; xx < int(input[0].size()); ++xx)
    {
        right++;
        if (input[y][xx] >= val)
        {
            break;
        }
    }
    for (int yy = y - 1; yy >= 0; --yy)
    {
        up++;
        if (input[yy][x] >= val)
        {
            break;
        }
    }
    for (int yy = y + 1; yy < int(input.size()); ++yy)
    {
        down++;
        if (input[yy][x] >= val)
        {
            break;
        }
    }
    return left * right * up * down;
}

uint64_t step2(const Input& input)
{
    size_t max = 0;
    for (int y = 0; y < int(input.size()); ++y)
    {
        for (int x = 0; x < int(input[0].size()); ++x)
        {
            uint64_t score = lines_of_sight(input, x, y);
            if (score > max)
            {
                max = score;
            }
        }
    }
    return max;
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
