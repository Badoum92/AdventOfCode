#include "common.h"

#include <deque>

const char* provided_paths[] = {"input/day16_provided"};
uint64_t provided_expected1[] = {1651};
uint64_t provided_expected2[] = {0};

const char* real_input = "input/day16";

struct Input
{
    std::unordered_map<std::string, std::vector<std::string>> edges;
    std::unordered_map<std::string, uint32_t> rates;
};

Input parse_input(const char* path)
{
    Input input;
    auto lines = input_to_lines(path);
    for (const auto& line : lines)
    {
        char name[3];
        uint32_t rate;
        sscanf(line.c_str(), "Valve %s has flow rate=%u;", name, &rate);
        input.rates[name] = rate;
        auto line_split = split(split(line, "valves ")[1], ", ");
        auto& edges = input.edges[name];
        for (const auto& edge : line_split)
        {
            edges.push_back(edge);
        }
    }
    return input;
}

uint64_t step1(const Input& input)
{

    uint64_t pressure = 0;
    uint64_t pressure_add = 0;
    std::deque<std::string> q;
    q.push_back("AA");
    std::unordered_set<std::string> opened;

    while (!q.empty())
    {
        std::string node = q.front();
        q.pop_front();

        pressure += pressure_add;

        for (const auto& edge : input.edges.at(node))
        {

        }


    }

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
