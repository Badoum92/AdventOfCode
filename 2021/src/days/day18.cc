#include "common.hh"

struct Pair
{
    Pair* parent = nullptr;
    std::shared_ptr<Pair> left_pair = nullptr;
    std::shared_ptr<Pair> right_pair = nullptr;
    uint16_t left_number = 0;
    uint16_t right_number = 0;
};

using PairPtr = std::shared_ptr<Pair>;

static PairPtr copy_pair(const PairPtr& pair)
{
    auto new_pair = std::make_shared<Pair>();
    if (pair->left_pair != nullptr)
    {
        new_pair->left_pair = copy_pair(pair->left_pair);
        new_pair->left_pair->parent = new_pair.get();
    }
    else
    {
        new_pair->left_number = pair->left_number;
    }
    if (pair->right_pair != nullptr)
    {
        new_pair->right_pair = copy_pair(pair->right_pair);
        new_pair->right_pair->parent = new_pair.get();
    }
    else
    {
        new_pair->right_number = pair->right_number;
    }
    return new_pair;
}

static PairPtr parse_line(const std::string& line, size_t& i)
{
    auto pair = std::make_shared<Pair>();
    char c = line[i++];
    if (c == '[')
    {
        pair->left_pair = parse_line(line, i);
        pair->left_pair->parent = pair.get();
    }
    else
    {
        pair->left_number = c - '0';
    }
    c = line[i++];
    assert(c == ',');
    c = line[i++];
    if (c == '[')
    {
        pair->right_pair = parse_line(line, i);
        pair->right_pair->parent = pair.get();
    }
    else
    {
        pair->right_number = c - '0';
    }
    while (i < line.size() && line[i] == ']')
    {
        ++i;
    }
    return pair;
}

static std::vector<PairPtr> parse_input(std::ifstream& input)
{
    auto lines = input_to_lines(input);
    std::vector<PairPtr> pairs(lines.size());
    for (size_t l = 0; l < lines.size(); ++l)
    {
        size_t i = 1;
        pairs[l] = parse_line(lines[l], i);
    }
    return pairs;
}

static uint16_t* get_left_number(Pair* pair)
{
    if (pair->parent == nullptr)
    {
        return nullptr;
    }
    if (pair->parent->left_pair == nullptr)
    {
        return &pair->parent->left_number;
    }
    if (pair->parent->left_pair.get() != pair)
    {
        auto left = pair->parent->left_pair;
        while (left->right_pair != nullptr)
        {
            left = left->right_pair;
        }
        return &left->right_number;
    }
    return get_left_number(pair->parent);
}

static uint16_t* get_right_number(Pair* pair)
{
    if (pair->parent == nullptr)
    {
        return nullptr;
    }
    if (pair->parent->right_pair == nullptr)
    {
        return &pair->parent->right_number;
    }
    if (pair->parent->right_pair.get() != pair)
    {
        auto right = pair->parent->right_pair;
        while (right->left_pair != nullptr)
        {
            right = right->left_pair;
        }
        return &right->left_number;
    }
    return get_right_number(pair->parent);
}

static bool explode_pair(PairPtr& pair, size_t depth = 1)
{
    if (depth == 5)
    {
        assert(pair->left_pair == nullptr);
        assert(pair->right_pair == nullptr);

        auto left = get_left_number(pair.get());
        if (left != nullptr)
        {
            *left += pair->left_number;
        }

        auto right = get_right_number(pair.get());
        if (right != nullptr)
        {
            *right += pair->right_number;
        }

        if (pair->parent->left_pair == pair)
        {
            pair->parent->left_number = 0;
            pair->parent->left_pair = nullptr;
        }
        else if (pair->parent->right_pair == pair)
        {
            pair->parent->right_number = 0;
            pair->parent->right_pair = nullptr;
        }

        return true;
    }

    bool exploded = false;
    if (pair->left_pair != nullptr)
    {
        exploded |= explode_pair(pair->left_pair, depth + 1);
    }
    if (!exploded && pair->right_pair != nullptr)
    {
        exploded |= explode_pair(pair->right_pair, depth + 1);
    }
    return exploded;
}

static bool split_pair(PairPtr& pair)
{
    bool splited = false;
    if (!splited && pair->left_pair == nullptr && pair->left_number > 9)
    {
        pair->left_pair = std::make_shared<Pair>();
        pair->left_pair->parent = pair.get();
        pair->left_pair->left_number = pair->left_number / 2;
        pair->left_pair->right_number = pair->left_number / 2 + (pair->left_number % 2);
        return true;
    }
    if (!splited && pair->left_pair != nullptr)
    {
        splited |= split_pair(pair->left_pair);
    }
    if (!splited && pair->right_pair == nullptr && pair->right_number > 9)
    {
        pair->right_pair = std::make_shared<Pair>();
        pair->right_pair->parent = pair.get();
        pair->right_pair->left_number = pair->right_number / 2;
        pair->right_pair->right_number = pair->right_number / 2 + (pair->right_number % 2);
        return true;
    }
    if (!splited && pair->right_pair != nullptr)
    {
        splited |= split_pair(pair->right_pair);
    }
    return splited;
}

static PairPtr add_pairs(const PairPtr& left, const PairPtr& right)
{
    auto pair = std::make_shared<Pair>();
    pair->left_pair = left;
    pair->left_pair->parent = pair.get();
    pair->right_pair = right;
    pair->right_pair->parent = pair.get();
    while (true)
    {
        if (explode_pair(pair))
        {
            continue;
        }
        if (split_pair(pair))
        {
            continue;
        }
        break;
    }
    return pair;
}

static uint64_t score_pair(const PairPtr& pair)
{
    uint64_t left = pair->left_pair == nullptr ? pair->left_number : score_pair(pair->left_pair);
    uint64_t right = pair->right_pair == nullptr ? pair->right_number : score_pair(pair->right_pair);
    return 3 * left + 2 * right;
}

static uint64_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto pairs = parse_input(input);
    auto pair = pairs[0];
    for (size_t i = 1; i < pairs.size(); ++i)
    {
        pair = add_pairs(pair, pairs[i]);
    }
    return score_pair(pair);
}

static uint64_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto pairs = parse_input(input);
    uint64_t max_score = 0;
    for (size_t i = 0; i < pairs.size(); ++i)
    {
        for (size_t j = 0; j < pairs.size(); ++j)
        {
            if (j == i)
            {
                continue;
            }
            uint64_t score = score_pair(add_pairs(copy_pair(pairs[i]), copy_pair(pairs[j])));
            if (score > max_score)
            {
                max_score = score;
            }
        }
    }
    return max_score;
}

void Day18::run()
{
    std::cout << "Day18:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;

    auto t0 = std::chrono::steady_clock::now();
    auto s1 = step1("../input/day18");
    auto t1 = std::chrono::steady_clock::now();
    auto s2 = step2("../input/day18");
    auto t2 = std::chrono::steady_clock::now();

    auto t_s1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto t_s2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "    step1: " << s1 << " (" << t_s1 << " ms)\n";
    std::cout << "    step2: " << s2 << " (" << t_s2 << " ms)\n\n";
}

bool Day18::sanity_check()
{
    uint64_t step1_expected = 4140;
    uint64_t step2_expected = 3993;
    bool ret = true;

    uint64_t step1_got = step1("../input/day18_provided");
    if (step1_got != step1_expected)
    {
        fprintf(stderr, "\n    step1 sanity check failed: expected %llu, got %llu\n", step1_expected, step1_got);
        ret = false;
    }

    uint64_t step2_got = step2("../input/day18_provided");
    if (step2_got != step2_expected)
    {
        fprintf(stderr, "\n    step2 sanity check failed: expected %llu, got %llu\n", step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
