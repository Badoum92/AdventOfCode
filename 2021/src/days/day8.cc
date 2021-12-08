#include "common.hh"

std::vector<std::vector<std::string>> parse_input(std::ifstream& input)
{
    auto lines = input_to_lines(input);
    std::vector<std::vector<std::string>> ret(lines.size());
    for (size_t i = 0; i < lines.size(); ++i)
    {
        ret[i] = split(lines[i], " ");
        ret[i].erase(ret[i].begin() + 10);
        for (auto& digit : ret[i])
        {
            std::sort(digit.begin(), digit.end());
        }
    }
    return ret;
}

static uint32_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = parse_input(input);

    uint32_t total = 0;
    for (const auto& line : lines)
    {
        for (size_t i = 0; i < 4; ++i)
        {
            size_t size = line[i + 10].size();
            total += size == 2 || size == 3 || size == 4 || size == 7;
        }
    }

    return total;
}

size_t intersection_size(const std::string& s1, const std::string& s2)
{
    static std::string tmp;
    tmp.resize(0);
    std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), std::back_inserter(tmp));
    return tmp.size();
}

std::array<std::string, 10> decode_digits(const std::vector<std::string>& digits_str)
{
    std::array<std::string, 10> ret;
    for (const auto& str : digits_str)
    {
        size_t size = str.size();
        if (size == 2)
            ret[1] = str;
        else if (size == 4)
            ret[4] = str;
        else if (size == 3)
            ret[7] = str;
        else if (size == 7)
            ret[8] = str;
    }

    for (size_t i = 0; i < 10; ++i)
    {
        const auto& digit = digits_str[i];
        size_t digit_size = digit.size();
        size_t inter_size_1 = intersection_size(digit, ret[1]);
        size_t inter_size_4 = intersection_size(digit, ret[4]);

        /*
        0 - size 6 - inter 1 = 2 - inter 4 = 3
        2 - size 5 - inter 1 = 1 - inter 4 = 2
        3 - size 5 - inter 1 = 2 - inter 4 = 3
        5 - size 5 - inter 1 = 1 - inter 4 = 3
        6 - size 6 - inter 1 = 1 - inter 4 = 3
        9 - size 6 - inter 1 = 2 - inter 4 = 4
        */

        if (digit_size == 6 && inter_size_1 == 2 && inter_size_4 == 3)
            ret[0] = digit;
        else if (digit_size == 5 && inter_size_1 == 1 && inter_size_4 == 2)
            ret[2] = digit;
        else if (digit_size == 5 && inter_size_1 == 2 && inter_size_4 == 3)
            ret[3] = digit;
        else if (digit_size == 5 && inter_size_1 == 1 && inter_size_4 == 3)
            ret[5] = digit;
        else if (digit_size == 6 && inter_size_1 == 1 && inter_size_4 == 3)
            ret[6] = digit;
        else if (digit_size == 6 && inter_size_1 == 2 && inter_size_4 == 4)
            ret[9] = digit;
    }

    return ret;
}

uint32_t decode_digit(const std::array<std::string, 10>& digits, const std::string& digit_str)
{
    for (size_t i = 0; i < 10; ++i)
    {
        if (digits[i] == digit_str)
        {
            return i;
        }
    }
    assert(false);
    return 10;
}

uint32_t decode_number(const std::array<std::string, 10>& digits, const std::vector<std::string>& line)
{
    uint32_t number = 0;
    for (size_t i = 0; i < 4; ++i)
    {
        number = number * 10 + decode_digit(digits, line[i + 10]);
    }
    return number;
}

static uint32_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = parse_input(input);

    uint32_t total = 0;
    for (const auto& line : lines)
    {
        auto digits = decode_digits(line);
        total += decode_number(digits, line);
    }

    return total;
}

void Day8::run()
{
    std::cout << "Day8:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;

    auto t0 = std::chrono::steady_clock::now();
    auto s1 = step1("../input/day8");
    auto t1 = std::chrono::steady_clock::now();
    auto s2 = step2("../input/day8");
    auto t2 = std::chrono::steady_clock::now();

    auto t_s1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto t_s2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "    step1: " << s1 << " (" << t_s1 << " ms)\n";
    std::cout << "    step2: " << s2 << " (" << t_s2 << " ms)\n\n";
}

bool Day8::sanity_check()
{
    uint32_t step1_expected = 26;
    uint32_t step2_expected = 61229;
    bool ret = true;

    uint32_t step1_got = step1("../input/day8_provided");
    if (step1_got != step1_expected)
    {
        fprintf(stderr, "\n    step1 sanity check failed: expected %u, got %u\n", step1_expected, step1_got);
        ret = false;
    }

    uint32_t step2_got = step2("../input/day8_provided");
    if (step2_got != step2_expected)
    {
        fprintf(stderr, "\n    step2 sanity check failed: expected %u, got %u\n", step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
