#include "common.hh"

static unsigned step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto data = input_to_lines(input);

    unsigned x = 0;
    unsigned y = 0;
    unsigned total = 0;

    while (y < data.size())
    {
        if (data[y][x] == '#')
            ++total;

        x = (x + 3) % data[0].size();
        ++y;
    }

    return total;
}

static unsigned step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto data = input_to_lines(input);

    std::vector<std::tuple<unsigned, unsigned, unsigned>> slopes;
    slopes.push_back(std::make_tuple<unsigned, unsigned, unsigned>(1, 1, 0));
    slopes.push_back(std::make_tuple<unsigned, unsigned, unsigned>(3, 1, 0));
    slopes.push_back(std::make_tuple<unsigned, unsigned, unsigned>(5, 1, 0));
    slopes.push_back(std::make_tuple<unsigned, unsigned, unsigned>(7, 1, 0));
    slopes.push_back(std::make_tuple<unsigned, unsigned, unsigned>(1, 2, 0));

    for (auto& slope : slopes)
    {
        unsigned x = 0;
        unsigned y = 0;

        while (y < data.size())
        {
            if (data[y][x] == '#')
                std::get<2>(slope)++;

            x = (x + std::get<0>(slope)) % data[0].size();
            y += std::get<1>(slope);
        }
    }

    unsigned total = 1;
    for (const auto& slope : slopes)
        total *= std::get<2>(slope);
    return total;
}

void Day3::run()
{
    std::cout << "Day3:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day3::sanity_check()
{
    unsigned step1_expected = 7;
    unsigned step2_expected = 336;
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
