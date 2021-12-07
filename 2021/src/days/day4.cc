#include "common.hh"

constexpr uint8_t MARKED = 255;

struct Board
{
    std::array<uint8_t, 25> numbers;
    std::array<uint8_t, 5> rows;
    std::array<uint8_t, 5> cols;
    uint32_t total = 0;
    bool won = false;

    void add_number(uint8_t number, size_t x, size_t y)
    {
        numbers[y * 5 + x] = number;
        total += number;
    }

    bool mark_number(uint8_t number)
    {
        for (size_t y = 0; y < 5; ++y)
        {
            for (size_t x = 0; x < 5; ++x)
            {
                if (numbers[y * 5 + x] == number)
                {
                    numbers[y * 5 + x] = MARKED;
                    total -= number;
                    uint8_t row = ++rows[y];
                    uint8_t col = ++cols[x];
                    won = won || row == 5 || col == 5;
                    return won;
                }
            }
        }
        return won;
    }

    uint32_t score(uint8_t number)
    {
        return total * number;
    }
};

std::pair<std::vector<uint8_t>, std::vector<Board>> parse_input(std::ifstream& input)
{
    auto lines = input_to_lines(input);
    auto numbers_str = split(lines[0], ",");

    auto ret = std::make_pair(std::vector<uint8_t>(numbers_str.size()), std::vector<Board>());
    auto& numbers = ret.first;
    auto& boards = ret.second;

    for (size_t i = 0; i < numbers_str.size(); ++i)
    {
        numbers[i] = std::stoi(numbers_str[i]);
    }

    for (size_t i = 2; i < lines.size(); i += 6)
    {
        auto& board = boards.emplace_back();
        for (size_t y = 0; y < 5; ++y)
        {
            auto line = split(lines[i + y], " ");
            for (size_t x = 0; x < 5; ++x)
            {
                board.add_number(std::stoi(line[x]), x, y);
            }
        }
    }

    return ret;
}

uint32_t play_untill_bingo(size_t& i, const std::vector<uint8_t>& numbers, std::vector<Board>& boards, bool remove)
{
    for (; i < numbers.size(); ++i)
    {
        uint8_t n = numbers[i];
        for (auto it = boards.begin(); it != boards.end();)
        {
            if (it->mark_number(n))
            {
                if (remove)
                {
                    it = boards.erase(it);
                    return 0;
                }
                else
                {
                    return it->score(n);
                }
            }
            else
            {
                ++it;
            }
        }
    }
    return 0;
}

static uint32_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto [numbers, boards] = parse_input(input);
    size_t i = 0;
    return play_untill_bingo(i, numbers, boards, false);
}

static uint32_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto [numbers, boards] = parse_input(input);
    size_t i = 0;
    while (boards.size() > 1)
    {
        play_untill_bingo(i, numbers, boards, true);
    }
    return play_untill_bingo(i, numbers, boards, false);
}

void Day4::run()
{
    std::cout << "Day4:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;

    auto t0 = std::chrono::steady_clock::now();
    auto s1 = step1("../input/day4");
    auto t1 = std::chrono::steady_clock::now();
    auto s2 = step2("../input/day4");
    auto t2 = std::chrono::steady_clock::now();

    auto t_s1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto t_s2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "    step1: " << s1 << " (" << t_s1 << " ms)\n";
    std::cout << "    step2: " << s2 << " (" << t_s2 << " ms)\n\n";
}

bool Day4::sanity_check()
{
    uint32_t step1_expected = 4512;
    uint32_t step2_expected = 1924;
    bool ret = true;

    uint32_t step1_got = step1("../input/day4_provided");
    if (step1_got != step1_expected)
    {
        fprintf(stderr, "\n    step1 sanity check failed: expected %u, got %u\n", step1_expected, step1_got);
        ret = false;
    }

    uint32_t step2_got = step2("../input/day4_provided");
    if (step2_got != step2_expected)
    {
        fprintf(stderr, "\n    step2 sanity check failed: expected %u, got %u\n", step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
