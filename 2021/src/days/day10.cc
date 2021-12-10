#include "common.hh"

static bool is_valid(char c, std::vector<char>& stack)
{
    if (c == '(' || c == '[' || c == '{' || c == '<')
    {
        stack.push_back(c);
        return true;
    }
    if (stack.empty())
    {
        return false;
    }
    char prev = stack.back();
    stack.pop_back();
    switch (c)
    {
    case ')':
        return prev == '(';
    case ']':
        return prev == '[';
    case '}':
        return prev == '{';
    case '>':
        return prev == '<';
    }
    return false;
}

static uint64_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);

    uint64_t parenthesis = 0;
    uint64_t square = 0;
    uint64_t curly = 0;
    uint64_t angle = 0;

    std::vector<char> stack;
    stack.reserve(120);

    for (const auto& line : lines)
    {
        stack.clear();
        for (char c : line)
        {
            if (!is_valid(c, stack))
            {
                switch (c)
                {
                case ')':
                    ++parenthesis;
                    break;
                case ']':
                    ++square;
                    break;
                case '}':
                    ++curly;
                    break;
                case '>':
                    ++angle;
                    break;
                }
                break;
            }
        }
    }

    return parenthesis * 3 + square * 57 + curly * 1197 + angle * 25137;
}

static uint64_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);

    std::vector<uint64_t> scores;
    scores.reserve(lines.size());

    std::vector<char> stack;
    stack.reserve(120);

    for (const auto& line : lines)
    {
        uint64_t score = 0;
        bool corrupted = false;
        stack.clear();
        for (char c : line)
        {
            if (!is_valid(c, stack))
            {
                corrupted = true;
                break;
            }
        }
        if (corrupted)
        {
            continue;
        }
        while (!stack.empty())
        {
            char c = stack.back();
            stack.pop_back();
            switch (c)
            {
            case '(':
                score = score * 5 + 1;
                break;
            case '[':
                score = score * 5 + 2;
                break;
            case '{':
                score = score * 5 + 3;
                break;
            case '<':
                score = score * 5 + 4;
                break;
            }
        }
        scores.insert(std::upper_bound(scores.begin(), scores.end(), score), score);
    }
    return scores[scores.size() / 2];
}

void Day10::run()
{
    std::cout << "Day10:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;

    auto t0 = std::chrono::steady_clock::now();
    auto s1 = step1("../input/day10");
    auto t1 = std::chrono::steady_clock::now();
    auto s2 = step2("../input/day10");
    auto t2 = std::chrono::steady_clock::now();

    auto t_s1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto t_s2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "    step1: " << s1 << " (" << t_s1 << " ms)\n";
    std::cout << "    step2: " << s2 << " (" << t_s2 << " ms)\n\n";
}

bool Day10::sanity_check()
{
    uint64_t step1_expected = 26397;
    uint64_t step2_expected = 288957;
    bool ret = true;

    uint64_t step1_got = step1("../input/day10_provided");
    if (step1_got != step1_expected)
    {
        fprintf(stderr, "\n    step1 sanity check failed: expected %llu, got %llu\n", step1_expected, step1_got);
        ret = false;
    }

    uint64_t step2_got = step2("../input/day10_provided");
    if (step2_got != step2_expected)
    {
        fprintf(stderr, "\n    step2 sanity check failed: expected %llu, got %llu\n", step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
