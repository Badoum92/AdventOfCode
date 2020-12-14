#include "common.hh"

#define SET_BIT(X, N) (X |= (1ULL << (N)))
#define CLR_BIT(X, N) (X &= ~(1ULL << (N)))
#define GET_BIT(X, N) ((X & (1ULL << (N))) != 0)
#define MASK_SIZE     36

static uint64_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);

    std::string mask_str;
    mask_str.resize(MASK_SIZE);

    uint64_t mask = 0;
    uint64_t x_mask = 0;

    uint64_t addr = 0;
    uint64_t val = 0;

    std::unordered_map<uint64_t, uint64_t> mem;

    for (const auto& line : lines)
    {
        if (sscanf(line.c_str(), "mask = %s", mask_str.data()) == 1)
        {
            mask = 0;
            x_mask = 0;
            for (unsigned i = 0; i < MASK_SIZE; ++i)
            {
                if (mask_str[i] == '1')
                    SET_BIT(mask, MASK_SIZE - 1 - i);
                else if (mask_str[i] == 'X')
                    SET_BIT(x_mask, MASK_SIZE - 1 - i);
            }
        }
        else if (sscanf(line.c_str(), "mem[%zu] = %zu", &addr, &val) == 2)
        {
            val |= mask;
            val &= (mask | x_mask);
            mem[addr] = val;
        }
    }

    uint64_t total = 0;
    for (const auto& [key, val] : mem)
        total += val;
    return total;
}

void gen_masks(uint64_t x_mask, std::vector<uint64_t>& masks)
{
    masks.push_back(0ULL);
    for (int i = MASK_SIZE - 1; i >= 0; --i)
    {
        for (uint64_t& m : masks)
        {
            m <<= 1;
        }
        if (GET_BIT(x_mask, i))
        {
            size_t size = masks.size();
            for (unsigned j = 0; j < size; ++j)
            {
                masks.push_back(masks[j] | 1);
            }
        }
    }
}

static uint64_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);

    std::string mask_str;
    mask_str.resize(MASK_SIZE);

    uint64_t mask = 0;
    uint64_t x_mask = 0;

    uint64_t addr = 0;
    uint64_t val = 0;

    std::unordered_map<uint64_t, uint64_t> mem;
    std::vector<uint64_t> masks;

    for (const auto& line : lines)
    {
        if (sscanf(line.c_str(), "mask = %s", mask_str.data()) == 1)
        {
            mask = 0;
            x_mask = 0;
            masks.clear();
            for (unsigned i = 0; i < MASK_SIZE; ++i)
            {
                if (mask_str[i] == '1')
                    SET_BIT(mask, MASK_SIZE - 1 - i);
                else if (mask_str[i] == 'X')
                    SET_BIT(x_mask, MASK_SIZE - 1 - i);
            }
            if (x_mask == 0)
                std::cout << "AAA\n";
            gen_masks(x_mask, masks);
        }
        else if (sscanf(line.c_str(), "mem[%zu] = %zu", &addr, &val) == 2)
        {
            addr |= mask;
            for (uint64_t m : masks)
            {
                uint64_t ones = m;
                uint64_t zeros = ~m & x_mask;
                mem[(addr | ones) & ~zeros] = val;
            }
        }
    }

    uint64_t total = 0;
    for (const auto& [key, val] : mem)
        total += val;
    return total;
}

void Day14::run()
{
    std::cout << "Day14:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day14::sanity_check()
{
    uint64_t step1_expected = 165;
    uint64_t step2_expected = 208;
    bool ret = true;

    uint64_t step1_got = step1(provided_file);
    if (step1_got != step1_expected)
    {
        fprintf(stderr,
                "\n    step1 sanity check failed: expected %zu, got %zu\n",
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
