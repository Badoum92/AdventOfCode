#include "common.hh"

static unsigned range_size = 0;

static unsigned find_weak_number_index(const std::vector<uint64_t>& numbers)
{
    for (unsigned n = range_size; n < numbers.size(); ++n)
    {
        bool found = false;
        for (unsigned i = n - range_size; i < n - 1; ++i)
        {
            uint64_t delta = numbers[n] - numbers[i];
            auto b = begin(numbers) + i + 1;
            auto e = begin(numbers) + n;
            if ((found = std::find(b, e, delta) != e))
                break;
        }
        if (!found)
            return n;
    }
    assert(false);
    return 0;
}

static uint64_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto numbers = input_tokens_to_T<uint64_t>(input);
    return numbers[find_weak_number_index(numbers)];
}

static uint64_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto numbers = input_tokens_to_T<uint64_t>(input);
    uint64_t number = numbers[find_weak_number_index(numbers)];

    unsigned i = 0;
    unsigned j = 1;
    uint64_t sum = numbers[i] + numbers[j];

    while (true)
    {
        if (sum > number)
        {
            sum -= numbers[i++];
        }
        else if (sum < number)
        {
            sum += numbers[++j];
        }
        else
        {
            auto b = begin(numbers) + i;
            auto e = begin(numbers) + j + 1;
            const auto& [min, max] = std::minmax_element(b, e);
            return *min + *max;
        }
    }
    assert(false);
    return 0;
}

void Day9::run()
{
    std::cout << "Day9:\n";
    std::cout << "    running sanity check...";
    range_size = 5;
    if (!sanity_check())
        return;
    range_size = 25;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day9::sanity_check()
{
    uint64_t step1_expected = 127;
    uint64_t step2_expected = 62;
    bool ret = true;

    uint64_t step1_got = step1(provided_file);
    if (step1_got != step1_expected)
    {
        fprintf(stderr,
                "\n    step1 sanity check failed: expected %zu, got %zu\n",
                step1_expected, step1_got);
        ret = false;
    }

    uint64_t step2_got = step2(provided_file);
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
