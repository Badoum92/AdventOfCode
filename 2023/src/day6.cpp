#include "common.h"

struct Input
{
    std::vector<uint32_t> times;
    std::vector<uint32_t> distances;
};

Input parse_input(const char* path)
{
    Input input;
    auto lines = input_to_lines(path);
    auto split_0 = split(lines[0], ":");
    auto split_1 = split(lines[1], ":");
    auto split_times = split(split_0[1], " ");
    auto split_distances = split(split_1[1], " ");
    for (const auto& time : split_times)
    {
        input.times.push_back(std::stoi(time));
    }
    for (const auto& distance : split_distances)
    {
        input.distances.push_back(std::stoi(distance));
    }
    return input;
}

uint64_t step1(const Input& input)
{
    uint64_t ret = 1;
    for (size_t i_race = 0; i_race < input.times.size(); ++i_race)
    {
        uint64_t ret_race = 0;
        for (size_t j = 1; j < input.times[i_race]; ++j)
        {
            uint64_t distance = j * (input.times[i_race] - j);
            if (distance > input.distances[i_race])
            {
                ret_race++;
            }
        }
        if (ret_race > 0)
        {
            ret *= ret_race;
        }
    }
    return ret;
}

uint64_t step2(const Input& input)
{
    std::string time_str;
    std::string distance_str;

    char buffer[16];
    for (const auto& t : input.times)
    {
        _itoa(t, buffer, 10);
        time_str += buffer;
    }
    for (const auto& d : input.distances)
    {
        _itoa(d, buffer, 10);
        distance_str += buffer;
    }

    uint64_t time = std::stoull(time_str);
    uint64_t distance = std::stoull(distance_str);

    uint64_t ret = 0;
    for (size_t j = 1; j < time; ++j)
    {
        uint64_t d = j * (time - j);
        if (d > distance)
        {
            ret++;
        }
    }
    return ret;
}

int main()
{
    {
        uint64_t expected1 = 288;
        uint64_t expected2 = 71503;
        Input input1 = parse_input("input/day6_provided");
        Input input2 = parse_input("input/day6_provided");
        auto [res1, time1] = time_function(step1, input1);
        auto [res2, time2] = time_function(step2, input2);

        std::cout << "    Step1: Expected: " << expected1 << "  |  Result: " << res1 << "  |  Time: " << time1
                  << "  |  " << (res1 == expected1 ? "OK" : "KO") << "\n";
        std::cout << "    Step2: Expected: " << expected2 << "  |  Result: " << res2 << "  |  Time: " << time2
                  << "  |  " << (res2 == expected2 ? "OK" : "KO") << "\n";

        if (res1 != expected1 || res2 != expected2)
        {
            return 1;
        }
    }

    Input input = parse_input("input/day6");
    auto [res1, time1] = time_function(step1, input);
    auto [res2, time2] = time_function(step2, input);
    std::cout << "\n=================================\n";
    std::cout << "Step1: Result: " << res1 << "  |  Time: " << time1 << " ms\n";
    std::cout << "Step2: Result: " << res2 << "  |  Time: " << time2 << " ms\n";
}
