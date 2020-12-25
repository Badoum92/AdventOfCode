#include "common.hh"

static unsigned step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto data = input_to_lines(input);

    unsigned total = 0;
    for (const auto& s : data)
    {
        unsigned min = 0;
        unsigned max = 0;
        char c;
        std::string password;
        password.resize(64);

        sscanf(s.c_str(), "%d-%d %c: %s", &min, &max, &c, password.data());

        unsigned char_count = std::count(begin(password), end(password), c);
        if (min <= char_count && char_count <= max)
            ++total;
    }

    return total;
}

static unsigned step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto data = input_to_lines(input);

    unsigned total = 0;
    for (const auto& s : data)
    {
        unsigned idx1 = 0;
        unsigned idx2 = 0;
        char c;
        std::string password;
        password.resize(64);

        sscanf(s.c_str(), "%d-%d %c: %s", &idx1, &idx2, &c, password.data());
        --idx1;
        --idx2;

        if (password[idx1] == c ^ password[idx2] == c)
            ++total;
    }

    return total;
}

void Day2::run()
{
    std::cout << "Day2:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day2::sanity_check()
{
    unsigned step1_expected = 2;
    unsigned step2_expected = 1;
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
