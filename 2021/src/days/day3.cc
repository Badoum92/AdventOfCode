#include "common.hh"

std::string find_most_common_bits(const std::vector<std::string>& lines)
{
    std::vector<uint32_t> ones(lines[0].size());
    for (const auto& line : lines)
    {
        for (size_t i = 0; i < line.size(); ++i)
        {
            ones[i] += line[i] == '1';
        }
    }

    std::string ret(ones.size(), '0');
    for (size_t i = 0; i < ones.size(); ++i)
    {
        if (ones[i] >= (lines.size() + 1) / 2)
        {
            ret[i] = '1';
        }
    }

    return ret;
}

uint32_t find_life_support_rating(std::vector<std::string>& lines, bool invert)
{
    for (size_t i = 0; lines.size() > 1; ++i)
    {
        std::string common_bits = find_most_common_bits(lines);

        if (invert)
        {
            for (char& b : common_bits)
            {
                b = b == '1' ? '0' : '1';
            }
        }

        for (auto it = lines.begin(); it != lines.end();)
        {
            const auto& line = *it;
            if (line[i] != common_bits[i])
            {
                it = lines.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
    return std::stoul(lines[0], nullptr, 2);
}

static uint32_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);

    uint32_t mask = 0;
    for (size_t i = 0; i < lines[0].size(); ++i)
    {
        mask |= 1 << i;
    }

    uint32_t gamma = std::stoul(find_most_common_bits(lines), nullptr, 2);
    return gamma * (~gamma & mask);
}

static uint32_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto o2_lines = input_to_lines(input);
    auto co2_lines = o2_lines;

    uint32_t o2 = find_life_support_rating(o2_lines, false);
    uint32_t co2 = find_life_support_rating(co2_lines, true);

    return o2 * co2;
}

void Day3::run()
{
    std::cout << "Day3:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;

    auto t0 = std::chrono::steady_clock::now();
    auto s1 = step1("../input/day3");
    auto t1 = std::chrono::steady_clock::now();
    auto s2 = step2("../input/day3");
    auto t2 = std::chrono::steady_clock::now();

    auto t_s1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto t_s2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "    step1: " << s1 << " (" << t_s1 << " ms)\n";
    std::cout << "    step2: " << s2 << " (" << t_s2 << " ms)\n\n";
}

bool Day3::sanity_check()
{
    uint32_t step1_expected = 198;
    uint32_t step2_expected = 230;
    bool ret = true;

    uint32_t step1_got = step1("../input/day3_provided");
    if (step1_got != step1_expected)
    {
        fprintf(stderr, "\n    step1 sanity check failed: expected %u, got %u\n", step1_expected, step1_got);
        ret = false;
    }

    uint32_t step2_got = step2("../input/day3_provided");
    if (step2_got != step2_expected)
    {
        fprintf(stderr, "\n    step2 sanity check failed: expected %u, got %u\n", step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
