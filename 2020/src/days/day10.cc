#include "common.hh"

static unsigned step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto numbers = input_to_tokens<unsigned>(input);
    std::sort(begin(numbers), end(numbers));

    unsigned one = numbers[0] == 1 ? 1 : 0;
    unsigned three = numbers[0] == 3 ? 2 : 1;
    for (unsigned i = 0; i < numbers.size() - 1; ++i)
    {
        unsigned n_1 = numbers[i];
        unsigned n_2 = numbers[i + 1];
        unsigned diff = n_2 - n_1;
        if (diff == 1)
        {
            ++one;
        }
        else if (diff == 3)
        {
            ++three;
        }
    }
    return one * three;
}

static uint64_t step2_rec(unsigned i, const std::vector<unsigned>& numbers,
                          std::vector<uint64_t>& cache)
{
    if (cache[i] != 0)
        return cache[i];

    if (i == numbers.size() - 1)
    {
        cache[i] = 1;
        return 1;
    }

    uint64_t count = step2_rec(i + 1, numbers, cache);
    if (i < numbers.size() - 2 && numbers[i + 2] - numbers[i] <= 3)
    {
        count += step2_rec(i + 2, numbers, cache);
    }
    if (i < numbers.size() - 3 && numbers[i + 3] - numbers[i] <= 3)
    {
        count += step2_rec(i + 3, numbers, cache);
    }

    cache[i] = count;
    return count;
}

static uint64_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto numbers = input_to_tokens<unsigned>(input);
    numbers.push_back(0);
    std::vector<uint64_t> cache(numbers.size(), 0);
    std::sort(begin(numbers), end(numbers));
    return step2_rec(0, numbers, cache);
}

void Day10::run()
{
    std::cout << "Day10:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day10::sanity_check()
{
    unsigned step1_expected = 35;
    uint64_t step2_expected = 19208;
    bool ret = true;

    unsigned step1_got = step1(provided_file);
    if (step1_got != step1_expected)
    {
        fprintf(stderr,
                "\n    step1 sanity check failed: expected %u, got %u\n",
                step1_expected, step1_got);
        ret = false;
    }

    uint64_t step2_got =
        step2((std::string(provided_file) + std::string("2")).c_str());
    if (step2_got != step2_expected)
    {
        fprintf(stderr,
                "\n    step2 sanity check failed: expected %zu, got %zu\n",
                step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
