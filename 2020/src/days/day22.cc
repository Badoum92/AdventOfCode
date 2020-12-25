#include "common.hh"

using Deck = std::deque<uint64_t>;
using Configuration = std::pair<unsigned, unsigned>;

static std::pair<Deck, Deck> parse_input(const std::vector<std::string>& lines)
{
    Deck d1;
    Deck d2;
    unsigned i = 1;
    for (; lines[i] != ""; ++i)
    {
        d1.push_back(std::stoul(lines[i]));
    }
    i += 2;
    for (; i < lines.size(); ++i)
    {
        d2.push_back(std::stoul(lines[i]));
    }
    return std::make_pair(d1, d2);
}

static uint64_t deck_score(const Deck& d)
{
    uint64_t score = 0;
    for (unsigned i = 0; i < d.size(); ++i)
    {
        score += d[i] * (d.size() - i);
    }
    return score;
}

static bool combat(Deck& d1, Deck& d2)
{
    while (!d1.empty() && !d2.empty())
    {
        unsigned c1 = d1[0];
        unsigned c2 = d2[0];
        d1.pop_front();
        d2.pop_front();

        if (c1 > c2)
        {
            d1.push_back(c1);
            d1.push_back(c2);
        }
        else
        {
            d2.push_back(c2);
            d2.push_back(c1);
        }
    }
    return d2.empty();
}

static uint64_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);
    auto [d1, d2] = parse_input(lines);

    bool winner = combat(d1, d2);

    if (winner)
        return deck_score(d1);
    return deck_score(d2);
}

static bool recursive_combat(Deck& d1, Deck& d2)
{
    std::unordered_set<uint64_t> seen;

    while (!d1.empty() && !d2.empty())
    {
        uint64_t cur = deck_score(d1) * 100000ULL + deck_score(d2);
        if (!seen.insert(cur).second)
            return true;

        bool round_winner = true;
        unsigned c1 = d1[0];
        unsigned c2 = d2[0];
        d1.pop_front();
        d2.pop_front();

        if (c1 <= d1.size() && c2 <= d2.size())
        {
            Deck d1_cpy{begin(d1), begin(d1) + c1};
            Deck d2_cpy{begin(d2), begin(d2) + c2};
            round_winner = recursive_combat(d1_cpy, d2_cpy);
        }
        else
        {
            round_winner = c1 > c2;
        }

        if (round_winner)
        {
            d1.push_back(c1);
            d1.push_back(c2);
        }
        else
        {
            d2.push_back(c2);
            d2.push_back(c1);
        }
    }
    return d2.empty();
}

static uint64_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);
    auto [d1, d2] = parse_input(lines);

    bool winner = recursive_combat(d1, d2);

    if (winner)
        return deck_score(d1);
    return deck_score(d2);
}

void Day22::run()
{
    std::cout << "Day22:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day22::sanity_check()
{
    uint64_t step1_expected = 306;
    uint64_t step2_expected = 291;
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
