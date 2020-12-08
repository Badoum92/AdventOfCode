#include "common.hh"

using bags_t =
    std::unordered_map<std::string, std::unordered_map<std::string, unsigned>>;

static bool bag_lookup(const std::string& bag, const bags_t& bags,
                       std::unordered_set<std::string>& set)
{
    if (set.count(bag))
        return true;

    if (bag == "shinygold")
        return true;

    if (bags.at(bag).size() == 0)
        return false;

    for (const auto& [key, val] : bags.at(bag))
    {
        if (bag_lookup(key, bags, set))
        {
            set.insert(bag);
            return true;
        }
    }

    return false;
}

static bags_t parse_bags(const std::vector<std::string>& data)
{
    bags_t bags;
    for (const auto& s : data)
    {
        auto words = split(s, " ");
        std::string color = words[0] + words[1];
        bags[color] = std::unordered_map<std::string, unsigned>();
        auto& bag = bags[color];

        if (words[4] == "no")
            continue;

        for (unsigned i = 4; i < words.size(); i += 4)
        {
            unsigned n = std::stoi(words[i]);
            color = words[i + 1] + words[i + 2];
            bag.emplace(color, n);
        }
    }
    return bags;
}

static unsigned step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto data = input_to_lines(input);
    auto bags = parse_bags(data);

    std::unordered_set<std::string> set;
    for (const auto& [k1, v1] : bags)
    {
        bag_lookup(k1, bags, set);
    }

    return set.size();
}

static unsigned count_bags(const std::string& bag, const bags_t& bags)
{
    unsigned total = 0;
    for (const auto& [key, val] : bags.at(bag))
    {
        total += val * (count_bags(key, bags) + 1);
    }
    return total;
}

static unsigned step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto data = input_to_lines(input);
    auto bags = parse_bags(data);
    return count_bags("shinygold", bags);
}

void Day7::run()
{
    std::cout << "Day7:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day7::sanity_check()
{
    unsigned step1_expected = 4;
    unsigned step2_expected = 126;
    bool ret = true;

    unsigned step1_got = step1(provided_file);
    if (step1_got != step1_expected)
    {
        fprintf(stderr,
                "\n    step1 sanity check failed: expected %u, got %u\n",
                step1_expected, step1_got);
        ret = false;
    }

    unsigned step2_got =
        step2((std::string(provided_file) + std::string("2")).c_str());
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
