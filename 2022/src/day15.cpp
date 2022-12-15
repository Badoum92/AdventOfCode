#include "common.h"

#include <optional>

const char* provided_paths[] = {"input/day15_provided"};
uint64_t provided_expected1[] = {26};
uint64_t provided_expected2[] = {56000011};

const char* real_input = "input/day15";

struct Position
{
    int32_t x;
    int32_t y;
};

struct Line
{
    int32_t begin;
    int32_t end;
};

struct Input
{
    std::vector<Position> sensors;
    std::vector<Position> beacons;
    std::vector<int32_t> distances;
    int32_t y_check;
    int32_t max;
};

Input parse_input(const char* path)
{
    Input input;
    auto lines = input_to_lines(path);
    input.sensors.reserve(lines.size() - 1);
    input.beacons.reserve(lines.size() - 1);
    input.distances.reserve(lines.size() - 1);
    input.y_check = std::stoi(lines[0]);
    input.max = std::stoi(lines[1]);

    for (size_t i = 2; i < lines.size(); ++i)
    {
        const auto& line = lines[i];
        auto& s = input.sensors.emplace_back();
        auto& b = input.beacons.emplace_back();
        auto& d = input.distances.emplace_back();
        sscanf(line.c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &s.x, &s.y, &b.x, &b.y);
        d = std::abs(s.x - b.x) + std::abs(s.y - b.y);
    }

    return input;
}

std::optional<Line> blocked_line(const Input& input, size_t i, int32_t y)
{
    const auto& s = input.sensors[i];
    const auto& d = input.distances[i];
    int32_t offset = d - std::abs(y - s.y);
    if (offset < 0)
    {
        return std::nullopt;
    }
    Line line;
    line.begin = s.x - offset;
    line.end = s.x + offset;
    return line;
}

void insert_line(std::vector<Line>& lines, const Line& to_insert)
{
    for (auto& line : lines)
    {
        if (line.begin <= to_insert.begin && to_insert.begin <= line.end)
        {
            line.end = std::max(line.end, to_insert.end);
            return;
        }
        else if (line.begin <= to_insert.end && to_insert.end <= line.end)
        {
            line.begin = std::min(line.begin, to_insert.begin);
            return;
        }
    }
    lines.push_back(to_insert);
}

std::vector<Line> non_overlapping_lines(std::vector<Line>& lines)
{
    std::sort(lines.begin(), lines.end(), [](const Line& a, const Line& b) { return a.begin < b.begin; });
    std::vector<Line> non_overlapping;
    non_overlapping.reserve(lines.size());
    non_overlapping.push_back(lines.front());
    for (size_t i = 1; i < lines.size(); ++i)
    {
        const auto& line = lines[i];
        auto& cur = non_overlapping.back();
        if (cur.end >= line.begin)
        {
            cur.end = std::max(cur.end, line.end);
        }
        else
        {
            non_overlapping.push_back(line);
        }
    }
    return non_overlapping;
}

std::vector<Line> blocked_lines(const Input& input, size_t y)
{
    std::vector<Line> lines;
    lines.reserve(input.sensors.size());
    for (size_t i = 0; i < input.sensors.size(); ++i)
    {
        auto line = blocked_line(input, i, y);
        if (!line)
        {
            continue;
        }
        lines.push_back(*line);
    }
    return non_overlapping_lines(lines);
}

std::optional<int32_t> find_empty(const std::vector<Line>& lines, int32_t max)
{
    for (const auto& line : lines)
    {
        if (line.begin > 0)
        {
            return line.begin - 1;
        }
        else if (line.end < max)
        {
            return line.end + 1;
        }
    }
    return std::nullopt;
}

uint64_t step1(const Input& input)
{
    std::vector<Line> lines = blocked_lines(input, input.y_check);
    uint64_t total = 0;
    for (const auto& line : lines)
    {
        total += line.end - line.begin;
    }
    return total;
}

uint64_t step2(const Input& input)
{
    for (int32_t y = 0; y <= input.max; ++y)
    {
        auto empty = find_empty(blocked_lines(input, y), input.max);
        if (!empty)
        {
            continue;
        }
        return *empty * 4000000ULL + y;
    }
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
