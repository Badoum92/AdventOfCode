#include "common.hh"

static unsigned step1(const char* input_file)
{
    std::ifstream input(input_file);
    std::unordered_set<char> set;
    std::string s;

    unsigned total = 0;
    while (std::getline(input, s))
    {
        if (s.empty())
        {
            total += set.size();
            set.clear();
            continue;
        }

        for (const auto& c : s)
        {
            set.insert(c);
        }
    }
    total += set.size();
    return total;
}

static unsigned step2(const char* input_file)
{
    std::ifstream input(input_file);
    std::unordered_map<char, unsigned> map;
    unsigned group_size = 0;
    std::string s;

    unsigned total = 0;
    while (std::getline(input, s))
    {
        if (s.empty())
        {
            for (const auto& [key, val] : map)
            {
                if (val == group_size)
                    ++total;
            }
            map.clear();
            group_size = 0;
            continue;
        }

        for (const auto& c : s)
        {
            map[c]++;
        }

        ++group_size;
    }

    for (const auto& [key, val] : map)
    {
        if (val == group_size)
            ++total;
    }

    return total;
}

void Day6::run()
{
    std::cout << "Day6:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day6::sanity_check()
{
    unsigned step1_expected = 11;
    unsigned step2_expected = 6;
    bool ret = true;

    unsigned step1_got = step1(provided_file);
    if (step1_got != step1_expected)
    {
        fprintf(stderr,
                "\n    step1 sanity check failed: expected %u, got %u\n",
                step1_expected, step1_got);
        ret = false;
    }

    unsigned step2_got = step2(provided_file);
    if (step2_got != step2_expected)
    {
        fprintf(stderr,
                "\n    step2 sanity check failed: expected %u, got %u\n",
                step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
