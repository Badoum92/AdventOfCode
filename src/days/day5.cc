#include "common.hh"

static unsigned step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto data = input_to_lines(input);

    unsigned max = 0;
    for (const auto& s : data)
    {
        unsigned row_min = 0;
        unsigned row_max = 127;
        unsigned row = 0;
        unsigned col_min = 0;
        unsigned col_max = 7;
        unsigned col = 0;

        for (unsigned i = 0; i < 6; ++i)
        {
            char c = s[i];
            if (c == 'F')
                row_max = (row_max + row_min) / 2;
            else if (c == 'B')
                row_min = (row_max + row_min + 1) / 2;
        }

        row = s[6] == 'F' ? row_min : row_max;

        for (unsigned i = 7; i < 9; ++i)
        {
            char c = s[i];
            if (c == 'L')
                col_max = (col_max + col_min) / 2;
            else if (c == 'R')
                col_min = (col_max + col_min + 1) / 2;
        }

        col = s[9] == 'L' ? col_min : col_max;

        unsigned id = row * 8 + col;
        max = std::max(max, id);
    }

    return max;
}

static unsigned step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto data = input_to_lines(input);

    std::set<unsigned> ids;
    for (const auto& s : data)
    {
        unsigned row_min = 0;
        unsigned row_max = 127;
        unsigned row = 0;
        unsigned col_min = 0;
        unsigned col_max = 7;
        unsigned col = 0;

        for (unsigned i = 0; i < 6; ++i)
        {
            char c = s[i];
            if (c == 'F')
                row_max = (row_max + row_min) / 2;
            else if (c == 'B')
                row_min = (row_max + row_min + 1) / 2;
        }

        row = s[6] == 'F' ? row_min : row_max;

        for (unsigned i = 7; i < 9; ++i)
        {
            char c = s[i];
            if (c == 'L')
                col_max = (col_max + col_min) / 2;
            else if (c == 'R')
                col_min = (col_max + col_min + 1) / 2;
        }

        col = s[9] == 'L' ? col_min : col_max;

        unsigned id = row * 8 + col;
        ids.insert(id);
    }

    unsigned min = *ids.begin();
    unsigned max = *ids.rbegin();

    for (unsigned i = min; i < max; ++i)
    {
        if (ids.count(i) == 0)
            return i;
    }

    return 0;
}

void Day5::run()
{
    std::cout << "Day5:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day5::sanity_check()
{
    unsigned step1_expected = 820;
    bool ret = true;

    unsigned step1_got = step1(provided_file);
    if (step1_got != step1_expected)
    {
        fprintf(stderr,
                "\n    step1 sanity check failed: expected %u, got %u\n",
                step1_expected, step1_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
