#include "common.h"

using Input = std::vector<std::string>;

Input parse_input(const char* path)
{
    return input_to_lines(path);
}

enum HandType : uint8_t
{
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    FIVE_OF_A_KIND,
};

struct Hand
{
    uint8_t cards[5];
    HandType type;
    uint32_t bid;
};

HandType cards_to_hand_type_1(uint8_t card_ranks[13])
{
    uint8_t three_of_a_kind = 0;
    uint8_t pair = 0;

    for (size_t i = 0; i < 13; ++i)
    {
        uint8_t n_cards = card_ranks[i];
        if (n_cards == 5)
        {
            return FIVE_OF_A_KIND;
        }
        else if (n_cards == 4)
        {
            return FOUR_OF_A_KIND;
        }
        else if (n_cards == 3)
        {
            three_of_a_kind++;
        }
        else if (n_cards == 2)
        {
            pair++;
        }
    }

    if (three_of_a_kind && pair)
    {
        return FULL_HOUSE;
    }
    else if (three_of_a_kind)
    {
        return THREE_OF_A_KIND;
    }
    else if (pair == 2)
    {
        return TWO_PAIR;
    }
    else if (pair == 1)
    {
        return ONE_PAIR;
    }
    return HIGH_CARD;
}

uint64_t hands_score(std::vector<Hand>& hands)
{
    std::sort(hands.begin(), hands.end(), [](const Hand& a, const Hand& b) {
        if (a.type != b.type)
        {
            return a.type < b.type;
        }
        for (size_t i = 0; i < 5; ++i)
        {
            if (a.cards[i] != b.cards[i])
            {
                return a.cards[i] < b.cards[i];
            }
        }
        return false;
    });
    uint64_t score = 0;
    for (size_t i = 0; i < hands.size(); ++i)
    {
        score += (i + 1) * hands[i].bid;
    }
    return score;
}

uint64_t step1(const Input& input)
{
    const std::unordered_map<char, uint8_t> card_to_ranks = {
        {'2', 0}, {'3', 1}, {'4', 2}, {'5', 3},  {'6', 4},  {'7', 5},  {'8', 6},
        {'9', 7}, {'T', 8}, {'J', 9}, {'Q', 10}, {'K', 11}, {'A', 12},
    };

    std::vector<Hand> hands;
    for (const auto& line : input)
    {
        Hand& hand = hands.emplace_back();
        auto line_split = split(line, " ");
        hand.bid = std::stoul(line_split[1]);
        for (size_t i = 0; i < 5; ++i)
        {
            hand.cards[i] = card_to_ranks.at(line_split[0][i]);
        }
        uint8_t card_ranks[13] = {0};
        for (size_t i = 0; i < 5; ++i)
        {
            card_ranks[hand.cards[i]]++;
        }
        hand.type = cards_to_hand_type_1(card_ranks);
    }

    return hands_score(hands);
}

HandType cards_to_hand_type_2(uint8_t card_ranks[13])
{
    uint8_t three_of_a_kind = 0;
    uint8_t pair = 0;
    uint8_t jokers = card_ranks[0];

    for (size_t i = 1; i < 13; ++i)
    {
        uint8_t n_cards = card_ranks[i];
        if (n_cards == 5)
        {
            return FIVE_OF_A_KIND;
        }
        else if (n_cards == 4)
        {
            return jokers == 1 ? FIVE_OF_A_KIND : FOUR_OF_A_KIND;
        }
        else if (n_cards == 3)
        {
            three_of_a_kind++;
        }
        else if (n_cards == 2)
        {
            pair++;
        }
    }

    if (three_of_a_kind && pair)
    {
        return FULL_HOUSE;
    }
    else if (three_of_a_kind)
    {
        if (jokers == 0)
        {
            return THREE_OF_A_KIND;
        }
        else if (jokers == 1)
        {
            return FOUR_OF_A_KIND;
        }
        else if (jokers == 2)
        {
            return FIVE_OF_A_KIND;
        }
    }
    else if (pair == 2)
    {
        return jokers == 1 ? FULL_HOUSE : TWO_PAIR;
    }
    else if (pair == 1)
    {
        if (jokers == 0)
        {
            return ONE_PAIR;
        }
        else if (jokers == 1)
        {
            return THREE_OF_A_KIND;
        }
        else if (jokers == 2)
        {
            return FOUR_OF_A_KIND;
        }
        else if (jokers == 3)
        {
            return FIVE_OF_A_KIND;
        }
    }

    if (jokers == 1)
    {
        return ONE_PAIR;
    }
    else if (jokers == 2)
    {
        return THREE_OF_A_KIND;
    }
    else if (jokers == 3)
    {
        return FOUR_OF_A_KIND;
    }
    else if (jokers == 4)
    {
        return FIVE_OF_A_KIND;
    }
    else if (jokers == 5)
    {
        return FIVE_OF_A_KIND;
    }

    return HIGH_CARD;
}

uint64_t step2(const Input& input)
{
    const std::unordered_map<char, uint8_t> card_to_ranks = {
        {'J', 0}, {'2', 1}, {'3', 2}, {'4', 3},  {'5', 4},  {'6', 5},  {'7', 6},
        {'8', 7}, {'9', 8}, {'T', 9}, {'Q', 10}, {'K', 11}, {'A', 12},
    };

    std::vector<Hand> hands;
    for (const auto& line : input)
    {
        Hand& hand = hands.emplace_back();
        auto line_split = split(line, " ");
        hand.bid = std::stoul(line_split[1]);
        for (size_t i = 0; i < 5; ++i)
        {
            hand.cards[i] = card_to_ranks.at(line_split[0][i]);
        }
        uint8_t card_ranks[13] = {0};
        for (size_t i = 0; i < 5; ++i)
        {
            card_ranks[hand.cards[i]]++;
        }
        hand.type = cards_to_hand_type_2(card_ranks);
    }

    return hands_score(hands);
}

int main()
{
    {
        uint64_t expected1 = 6440;
        uint64_t expected2 = 5905;
        Input input1 = parse_input("input/day7_provided");
        Input input2 = parse_input("input/day7_provided");
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

    Input input = parse_input("input/day7");
    auto [res1, time1] = time_function(step1, input);
    auto [res2, time2] = time_function(step2, input);
    std::cout << "\n=================================\n";
    std::cout << "Step1: Result: " << res1 << "  |  Time: " << time1 << " ms\n";
    std::cout << "Step2: Result: " << res2 << "  |  Time: " << time2 << " ms\n";
}
