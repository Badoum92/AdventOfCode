#include "common.h"

struct Point
{
    uint32_t x = 0;
    uint32_t y = 0;
};

struct Gear
{
    uint32_t x = 0;
    uint32_t y = 0;
    std::vector<uint32_t> numbers;
};

using Input = std::vector<std::string>;

Input parse_input(const char* path)
{
    return input_to_lines(path);
}

bool is_in_grid(const Input& input, const Point& p)
{
    return p.y < input.size() && p.x < input[p.y].size();
}

uint64_t step1(const Input& input)
{
    uint64_t sum = 0;
    for (uint32_t y = 0; y < input.size(); ++y)
    {
        const auto& line = input[y];
        for (uint32_t x = 0; x < line.size(); ++x)
        {
            if (isdigit(line[x]))
            {
                std::vector<Point> adjacent;
                uint64_t number = line[x] - '0';
                adjacent.push_back(Point{x - 1, y - 1});
                adjacent.push_back(Point{x - 1, y});
                adjacent.push_back(Point{x - 1, y + 1});
                adjacent.push_back(Point{x, y - 1});
                adjacent.push_back(Point{x, y + 1});
                ++x;
                while (isdigit(line[x]))
                {
                    number = number * 10 + line[x] - '0';
                    adjacent.push_back(Point{x, y - 1});
                    adjacent.push_back(Point{x, y + 1});
                    ++x;
                }
                adjacent.push_back(Point{x, y - 1});
                adjacent.push_back(Point{x, y});
                adjacent.push_back(Point{x, y + 1});

                for (const auto& p : adjacent)
                {
                    if (is_in_grid(input, p) && input[p.y][p.x] != '.')
                    {
                        sum += number;
                        break;
                    }
                }
            }
        }
    }
    return sum;
}

uint64_t step2(const Input& input)
{
    std::vector<Gear> gears;
    for (uint32_t y = 0; y < input.size(); ++y)
    {
        const auto& line = input[y];
        for (uint32_t x = 0; x < line.size(); ++x)
        {
            if (isdigit(line[x]))
            {
                std::vector<Point> adjacent;
                uint64_t number = line[x] - '0';
                adjacent.push_back(Point{x - 1, y - 1});
                adjacent.push_back(Point{x - 1, y});
                adjacent.push_back(Point{x - 1, y + 1});
                adjacent.push_back(Point{x, y - 1});
                adjacent.push_back(Point{x, y + 1});
                ++x;
                while (isdigit(line[x]))
                {
                    number = number * 10 + line[x] - '0';
                    adjacent.push_back(Point{x, y - 1});
                    adjacent.push_back(Point{x, y + 1});
                    ++x;
                }
                adjacent.push_back(Point{x, y - 1});
                adjacent.push_back(Point{x, y});
                adjacent.push_back(Point{x, y + 1});

                for (const auto& p : adjacent)
                {
                    if (is_in_grid(input, p) && input[p.y][p.x] == '*')
                    {
                        Gear* gear = nullptr;
                        for (auto& g : gears)
                        {
                            if (g.x == p.x && g.y == p.y)
                            {
                                gear = &g;
                                break;
                            }
                        }
                        if (!gear)
                        {
                            gear = &gears.emplace_back();
                        }
                        gear->x = p.x;
                        gear->y = p.y;
                        gear->numbers.push_back(number);
                    }
                }
            }
        }
    }

    uint64_t sum = 0;
    for (auto& g : gears)
    {
        if (g.numbers.size() == 2)
        {
            sum += g.numbers[0] * g.numbers[1];
        }
    }
    return sum;
}

int main()
{
    {
        uint64_t expected1 = 4361;
        uint64_t expected2 = 467835;
        Input input1 = parse_input("input/day3_provided");
        Input input2 = parse_input("input/day3_provided");
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

    Input input = parse_input("input/day3");
    auto [res1, time1] = time_function(step1, input);
    auto [res2, time2] = time_function(step2, input);
    std::cout << "\n=================================\n";
    std::cout << "Step1: Result: " << res1 << "  |  Time: " << time1 << " ms\n";
    std::cout << "Step2: Result: " << res2 << "  |  Time: " << time2 << " ms\n";
}
