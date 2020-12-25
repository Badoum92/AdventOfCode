#include "common.hh"

const uint64_t DIVIDE_VAL = 20201227;

static uint64_t get_loop_size(uint64_t public_key)
{
    uint64_t subject = 7;
    uint64_t value = 1;
    uint64_t loop_size = 0;
    while (value != public_key)
    {
        value *= subject;
        value %= DIVIDE_VAL;
        ++loop_size;
    }
    return loop_size;
}

static uint64_t transform(uint64_t subject, uint64_t loop_size)
{
    uint64_t value = 1;
    for (uint64_t i = 0; i < loop_size; ++i)
    {
        value *= subject;
        value %= DIVIDE_VAL;
    }
    return value;
}

static uint64_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    uint64_t card_key;
    uint64_t door_key;
    input >> card_key >> door_key;

    uint64_t card_loop_size = get_loop_size(card_key);
    uint64_t encryption_key = transform(door_key, card_loop_size);

    return encryption_key;
}

static uint64_t step2(const char* input_file)
{
    return 0;
}

void Day25::run()
{
    std::cout << "Day25:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day25::sanity_check()
{
    uint64_t step1_expected = 14897079;
    uint64_t step2_expected = 0;
    bool ret = true;

    uint64_t step1_got = step1(provided_file);
    if (step1_got != step1_expected)
    {
        fprintf(stderr, "\n    step1 sanity check failed: expected %zu, got %zu\n",
                step1_expected, step1_got);
        ret = false;
    }

    uint64_t step2_got = step2(provided_file);
    if (step2_got != step2_expected)
    {
        fprintf(stderr, "\n    step2 sanity check failed: expected %zu, got %zu\n",
                step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
