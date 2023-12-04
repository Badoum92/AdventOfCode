#include "common.h"

struct Card
{
    std::bitset<100> winning;
    std::bitset<100> have;
};

using Input = std::vector<Card>;

Input parse_input(const char* path)
{
    Input input;
    auto lines = input_to_lines(path);
    for (const auto& line : lines)
    {
        auto numbers = split(line, ": ")[1];
        auto sets = split(numbers, " | ");
        auto winning = split(sets[0], " ");
        auto have = split(sets[1], " ");
        Card& card = input.emplace_back();
        for (const auto& w : winning)
        {
            card.winning.set(atoi(w.c_str()), 1);
        }
        for (const auto& h : have)
        {
            card.have.set(atoi(h.c_str()), 1);
        }
    }
    return input;
}

uint64_t card_score(const Card& card)
{
    uint64_t count = (card.have & card.winning).count();
    return (1 * (count > 0)) << (count - 1);
}

uint64_t step1(const Input& input)
{
    uint64_t score = 0;
    for (const auto& card : input)
    {
        score += card_score(card);
    }
    return score;
}

uint64_t step2(const Input& input)
{
    uint64_t total = 0;
    std::vector<uint64_t> card_counts(input.size(), 1);
    for (size_t i = 0; i < input.size(); ++i)
    {
        total += card_counts[i];
        uint64_t score = card_score(input[i]);
        if (score != 0)
        {
            uint64_t n_matches = std::countr_zero(score) + 1;
            for (size_t j = 1; j <= n_matches; ++j)
            {
                card_counts[i + j] += card_counts[i];
            }
        }
    }
    return total;
}

int main()
{
    {
        uint64_t expected1 = 13;
        uint64_t expected2 = 30;
        Input input1 = parse_input("input/day4_provided");
        Input input2 = parse_input("input/day4_provided");
        auto [res1, time1] = time_function(step1, input1);
        auto [res2, time2] = time_function(step2, input2);

        std::cout << "    Step1: Expected: " << expected1 << "  |  Result: " << res1 << "  |  Time: " << time1
                  << "  |  " << (res1 == expected1 ? "OK" : "KO") << "\n";
        std::cout << "    Step2: Expected: " << expected2 << "  |  Result: " << res2 << "  |  Time: " << time2
                  << "  |  " << (res2 == expected2 ? "OK" : "KO") << "\n";

        if (res1 != expected1 || res2 != expected2)
        {
            return 1;
        }
    }

    Input input = parse_input("input/day4");
    auto [res1, time1] = time_function(step1, input);
    auto [res2, time2] = time_function(step2, input);
    std::cout << "\n=================================\n";
    std::cout << "Step1: Result: " << res1 << "  |  Time: " << time1 << " ms\n";
    std::cout << "Step2: Result: " << res2 << "  |  Time: " << time2 << " ms\n";
}
