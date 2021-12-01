#include "common.hh"

static unsigned step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto data = input_to_tokens<int>(input);

    for (unsigned i = 0; i < data.size() - 1; ++i)
    {
        for (unsigned j = i + 1; j < data.size(); ++j)
        {
            if (data[i] + data[j] == 2020)
            {
                return data[i] * data[j];
            }
        }
    }

    return 0;
}

static unsigned step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto data = input_to_tokens<int>(input);

    for (unsigned i = 0; i < data.size() - 2; ++i)
    {
        for (unsigned j = i + 1; j < data.size() - 1; ++j)
        {
            for (unsigned k = j + 1; k < data.size(); ++k)
            {
                if (data[i] + data[j] + data[k] == 2020)
                {
                    return data[i] * data[j] * data[k];
                }
            }
        }
    }

    return 0;
}

void Day1::run()
{
    std::cout << "Day1:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day1::sanity_check()
{
    unsigned step1_expected = 514579;
    unsigned step2_expected = 241861950;
    bool ret = true;

    unsigned step1_got = step1(provided_file);
    if (step1_got != step1_expected)
    {
        fprintf(stderr, "\n    step1 sanity check failed: expected %u, got %u\n",
                step1_expected, step1_got);
        ret = false;
    }

    unsigned step2_got = step2(provided_file);
    if (step2_got != step2_expected)
    {
        fprintf(stderr, "\n    step2 sanity check failed: expected %u, got %u\n",
                step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
