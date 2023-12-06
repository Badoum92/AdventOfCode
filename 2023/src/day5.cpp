#include "common.h"

struct Range
{
    int64_t begin;
    int64_t end;
    int64_t offset;
};

struct Input
{
    std::vector<int64_t> seeds;
    std::vector<std::vector<Range>> maps;
};

Input parse_input(const char* path)
{
    Input input;
    auto lines = input_to_lines(path);

    auto seeds = split(lines[0], " ");
    for (size_t i = 1; i < seeds.size(); ++i)
    {
        input.seeds.push_back(std::stoull(seeds[i]));
    }

    for (size_t i = 3; i < lines.size(); i += 2)
    {
        auto& map = input.maps.emplace_back();
        while (i < lines.size() && lines[i] != "")
        {
            auto range = split(lines[i], " ");
            int64_t dst = std::stoll(range[0]);
            int64_t src = std::stoll(range[1]);
            int64_t size = std::stoll(range[2]);
            map.push_back(Range{src, src + size, dst - src});
            i++;
        }
    }

    for (auto& map : input.maps)
    {
        std::sort(map.begin(), map.end(), [](Range& a, Range& b) { return a.begin < b.begin; });
    }

    for (auto& map : input.maps)
    {
        for (size_t i = 0; i < map.size() - 1; ++i)
        {
            if (map[i].end < map[i + 1].begin)
            {
                map.insert(map.begin() + i + 1, Range{map[i].end, map[i + 1].begin, 0});
            }
        }
    }

    return input;
}

int64_t src_to_dst(const std::vector<Range>& map, int64_t src)
{
    for (size_t i = 0; i < map.size(); ++i)
    {
        if (map[i].begin <= src && src < map[i].end)
        {
            return src + map[i].offset;
        }
    }
    return src;
}

int64_t seed_to_location(const Input& input, int64_t seed)
{
    int64_t soil = src_to_dst(input.maps[0], seed);
    int64_t fertilizer = src_to_dst(input.maps[1], soil);
    int64_t water = src_to_dst(input.maps[2], fertilizer);
    int64_t light = src_to_dst(input.maps[3], water);
    int64_t temperature = src_to_dst(input.maps[4], light);
    int64_t humidity = src_to_dst(input.maps[5], temperature);
    int64_t location = src_to_dst(input.maps[6], humidity);
    return location;
}

int64_t step1(const Input& input)
{
    int64_t min_location = INT64_MAX;
    for (int64_t seed : input.seeds)
    {
        min_location = std::min(seed_to_location(input, seed), min_location);
    }
    return min_location;
}

std::vector<Range> src_range_to_dst_ranges(const std::vector<Range>& map, int64_t src, int64_t size)
{
    std::vector<Range> ranges;
    int64_t current_offset = 0;
    for (size_t i = 0; i < map.size() && current_offset < size; ++i)
    {
        auto& range = map[i];
        if (range.begin <= src + current_offset)
        {
            int64_t range_size = std::min(size - current_offset, range.end - range.begin);
            ranges.push_back(Range{src + range.offset, src + range_size + range.offset, 0});
            current_offset += range_size;
        }
    }
    if (current_offset != size)
    {
        ranges.push_back(Range{src + current_offset, src + size, 0});
    }
    return ranges;
}

int64_t seed_range_to_location(const Input& input, int64_t begin, int64_t size)
{
    std::vector<Range> soil_ranges = src_range_to_dst_ranges(input.maps[0], begin, size);
    std::vector<Range> fertilizer_ranges;
    std::vector<Range> water_ranges;
    std::vector<Range> light_ranges;
    std::vector<Range> temperature_ranges;
    std::vector<Range> humidity_ranges;
    std::vector<Range> location_ranges;
    for (const auto& range : soil_ranges)
    {
        std::vector<Range> new_range = src_range_to_dst_ranges(input.maps[1], range.begin, range.end - range.begin);
        fertilizer_ranges.insert(fertilizer_ranges.end(), new_range.begin(), new_range.end());
    }
    for (const auto& range : fertilizer_ranges)
    {
        std::vector<Range> new_range = src_range_to_dst_ranges(input.maps[2], range.begin, range.end - range.begin);
        water_ranges.insert(water_ranges.end(), new_range.begin(), new_range.end());
    }
    for (const auto& range : water_ranges)
    {
        std::vector<Range> new_range = src_range_to_dst_ranges(input.maps[3], range.begin, range.end - range.begin);
        light_ranges.insert(light_ranges.end(), new_range.begin(), new_range.end());
    }
    for (const auto& range : light_ranges)
    {
        std::vector<Range> new_range = src_range_to_dst_ranges(input.maps[4], range.begin, range.end - range.begin);
        temperature_ranges.insert(temperature_ranges.end(), new_range.begin(), new_range.end());
    }
    for (const auto& range : temperature_ranges)
    {
        std::vector<Range> new_range = src_range_to_dst_ranges(input.maps[5], range.begin, range.end - range.begin);
        humidity_ranges.insert(humidity_ranges.end(), new_range.begin(), new_range.end());
    }
    for (const auto& range : humidity_ranges)
    {
        std::vector<Range> new_range = src_range_to_dst_ranges(input.maps[6], range.begin, range.end - range.begin);
        location_ranges.insert(location_ranges.end(), new_range.begin(), new_range.end());
    }
    int64_t location = INT64_MAX;
    for (const auto& range : location_ranges)
    {
        location = std::min(location, range.begin);
    }
    return location;
}

int64_t step2(const Input& input)
{
    int64_t min_location = INT64_MAX;
    for (size_t i = 0; i < input.seeds.size(); i += 2)
    {
        min_location = std::min(min_location, seed_range_to_location(input, input.seeds[i], input.seeds[i + 1]));
    }
    return min_location;
}

int main()
{
    {
        int64_t expected1 = 35;
        int64_t expected2 = 46;
        Input input1 = parse_input("input/day5_provided");
        Input input2 = parse_input("input/day5_provided");
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

    Input input = parse_input("input/day5");
    auto [res1, time1] = time_function(step1, input);
    auto [res2, time2] = time_function(step2, input);
    std::cout << "\n=================================\n";
    std::cout << "Step1: Result: " << res1 << "  |  Time: " << time1 << " ms\n";
    std::cout << "Step2: Result: " << res2 << "  |  Time: " << time2 << " ms\n";
}
