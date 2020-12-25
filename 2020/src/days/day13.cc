#include "common.hh"

static unsigned step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);
    unsigned earliest = std::stoi(lines[0]);
    auto str_ids = split(lines[1], ",");
    std::vector<unsigned> ids;
    for (const auto& s : str_ids)
    {
        if (s == "x")
            continue;
        ids.push_back(std::stoul(s));
    }
    unsigned min = std::numeric_limits<unsigned>::max();
    unsigned min_id = min;

    for (const auto& id : ids)
    {
        unsigned val = std::ceil((double)earliest / id) * id;
        if (val < min)
        {
            min = val;
            min_id = id;
        }
    }

    return (min - earliest) * min_id;
}

static uint64_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);
    auto str_ids = split(lines[1], ",");
    std::vector<std::pair<unsigned, unsigned>> ids;
    for (unsigned i = 0; i < str_ids.size(); ++i)
    {
        if (str_ids[i] == "x")
            continue;
        ids.emplace_back(std::stoul(str_ids[i]), i);
    }

    uint64_t cur = 0;
    uint64_t step = 1;
    for (const auto& id : ids)
    {
        while ((cur + id.second) % id.first != 0)
        {
            cur += step;
        }
        step *= id.first;
    }
    return cur;
}

void Day13::run()
{
    std::cout << "Day13:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day13::sanity_check()
{
    unsigned step1_expected = 295;
    uint64_t step2_expected = 1068781;
    bool ret = true;

    unsigned step1_got = step1(provided_file);
    if (step1_got != step1_expected)
    {
        fprintf(stderr,
                "\n    step1 sanity check failed: expected %u, got %u\n",
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
