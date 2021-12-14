#include "common.hh"

using Polymer = std::unordered_map<std::string, uint64_t>;

static std::string original_polymer;

static std::pair<Polymer, std::unordered_map<std::string, char>> parse_input(std::ifstream& input)
{
    auto lines = input_to_lines(input);

    std::pair<Polymer, std::unordered_map<std::string, char>> ret;
    auto& polymer = ret.first;
    auto& map = ret.second;

    original_polymer = lines[0];
    for (size_t i = 0; i < lines[0].size() - 1; ++i)
    {
        polymer[lines[0].substr(i, 2)]++;
    }

    for (size_t i = 2; i < lines.size(); ++i)
    {
        const auto& line = lines[i];
        auto s = split(line, " -> ");
        map.emplace(s[0], s[1][0]);
    }

    return ret;
}

static void step(Polymer& polymer, const std::unordered_map<std::string, char>& map)
{
    Polymer other;
    for (const auto& [key, val] : polymer)
    {
        auto it = map.find(key);
        if (it == map.end())
        {
            continue;
        }

        std::string new_pair1(2, key[0]);
        new_pair1[1] = it->second;
        std::string new_pair2(2, it->second);
        new_pair2[1] = key[1];

        other[new_pair1] += val;
        other[new_pair2] += val;
    }
    std::swap(polymer, other);
}

static void step_for(Polymer& polymer, const std::unordered_map<std::string, char>& map, size_t steps)
{
    for (size_t i = 0; i < steps; ++i)
    {
        step(polymer, map);
    }
}

static std::pair<uint64_t, uint64_t> find_min_max(const std::unordered_map<char, uint64_t>& map)
{
    uint64_t min = UINT64_MAX;
    uint64_t max = 0;
    for (const auto& [key, val] : map)
    {
        min = val < min ? val : min;
        max = val > max ? val : max;
    }
    return std::make_pair(min, max);
}

static uint64_t score(const Polymer& polymer)
{
    std::unordered_map<char, uint64_t> counts;
    for (const auto& [key, val] : polymer)
    {
        counts[key[0]] += val;
    }
    counts[original_polymer.back()]++;
    auto [min, max] = find_min_max(counts);
    return max - min;
}

static uint64_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto [polymer, map] = parse_input(input);
    step_for(polymer, map, 10);
    return score(polymer);
}

static uint64_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto [polymer, map] = parse_input(input);
    step_for(polymer, map, 40);
    return score(polymer);
}

void Day14::run()
{
    std::cout << "Day14:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;

    auto t0 = std::chrono::steady_clock::now();
    auto s1 = step1("../input/day14");
    auto t1 = std::chrono::steady_clock::now();
    auto s2 = step2("../input/day14");
    auto t2 = std::chrono::steady_clock::now();

    auto t_s1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto t_s2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "    step1: " << s1 << " (" << t_s1 << " ms)\n";
    std::cout << "    step2: " << s2 << " (" << t_s2 << " ms)\n\n";
}

bool Day14::sanity_check()
{
    uint64_t step1_expected = 1588;
    uint64_t step2_expected = 2188189693529;
    bool ret = true;

    uint64_t step1_got = step1("../input/day14_provided");
    if (step1_got != step1_expected)
    {
        fprintf(stderr, "\n    step1 sanity check failed: expected %llu, got %llu\n", step1_expected, step1_got);
        ret = false;
    }

    uint64_t step2_got = step2("../input/day14_provided");
    if (step2_got != step2_expected)
    {
        fprintf(stderr, "\n    step2 sanity check failed: expected %llu, got %llu\n", step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
