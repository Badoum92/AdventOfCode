#include "common.h"

struct CubeSet
{
    uint32_t r = 0;
    uint32_t g = 0;
    uint32_t b = 0;
};
using Game = std::vector<CubeSet>;
using Input = std::vector<Game>;

Input parse_input(const char* path)
{
    Input input;
    auto lines = input_to_lines(path);
    for (size_t i = 0; i < lines.size(); ++i)
    {
        Game& game = input.emplace_back();
        auto game_split = split(lines[i], ":")[1];
        auto set_split = split(game_split, ";");
        for (const auto& set : set_split)
        {
            CubeSet& cube_set = game.emplace_back();
            auto color_split = split(set, ",");
            for (const auto& color : color_split)
            {
                uint32_t color_number;
                char color_name[8] = {};
                sscanf(color.c_str(), "%u %s", &color_number, color_name);
                if (!strcmp(color_name, "red"))
                {
                    cube_set.r = color_number;
                }
                else if (!strcmp(color_name, "green"))
                {
                    cube_set.g = color_number;
                }
                else if (!strcmp(color_name, "blue"))
                {
                    cube_set.b = color_number;
                }
            }
        }
    }
    return input;
}

uint64_t step1(const Input& input)
{
    uint32_t max_red = 12;
    uint32_t max_green = 13;
    uint32_t max_blue = 14;

    uint64_t n = 0;
    for (size_t i = 0; i < input.size(); ++i)
    {
        bool possible = true;
        const Game& game = input[i];
        for (const auto& set : game)
        {
            if (set.r > max_red || set.g > max_green || set.b > max_blue)
            {
                possible = false;
                break;
            }
        }
        if (possible)
        {
            n += i + 1;
        }
    }
    return n;
}

uint64_t step2(const Input& input)
{
    uint64_t n = 0;
    for (size_t i = 0; i < input.size(); ++i)
    {
        CubeSet min_set;
        const Game& game = input[i];
        for (const auto& set : game)
        {
            min_set.r = std::max(min_set.r, set.r);
            min_set.g = std::max(min_set.g, set.g);
            min_set.b = std::max(min_set.b, set.b);
        }
        n += min_set.r * min_set.g * min_set.b;
    }
    return n;
}

int main()
{
    {
        uint64_t expected1 = 8;
        uint64_t expected2 = 2286;
        Input input1 = parse_input("input/day2_provided");
        Input input2 = parse_input("input/day2_provided");
        auto [res1, time1] = time_function(step1, input1);
        auto [res2, time2] = time_function(step2, input2);

        std::cout << "    Step1: Expected: " << expected1 << "  |  Result: " << res1 << "  |  Time: " << time1 << "  |  "
                  << (res1 == expected1 ? "OK" : "KO") << "\n";
        std::cout << "    Step2: Expected: " << expected2 << "  |  Result: " << res2 << "  |  Time: " << time2 << "  |  "
                  << (res2 == expected2 ? "OK" : "KO") << "\n";

        if (res1 != expected1 || res2 != expected2)
        {
            return 1;
        }
    }

    Input input = parse_input("input/day2");
    auto [res1, time1] = time_function(step1, input);
    auto [res2, time2] = time_function(step2, input);
    std::cout << "\n=================================\n";
    std::cout << "Step1: Result: " << res1 << "  |  Time: " << time1 << " ms\n";
    std::cout << "Step2: Result: " << res2 << "  |  Time: " << time2 << " ms\n";
}
