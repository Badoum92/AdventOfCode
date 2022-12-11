#include "common.h"

const char* provided_paths[] = {"input/day9_provided", "input/day9_provided2"};
uint64_t provided_expected1[] = {13, 88};
uint64_t provided_expected2[] = {1, 36};

const char* real_input = "input/day9";

struct Move
{
    char direction;
    uint8_t amount;
};

using Input = std::vector<Move>;
using Position = std::pair<int32_t, int32_t>;

template <>
struct std::hash<Position>
{
    size_t operator()(const Position& pos) const
    {
        return pos.first ^ pos.second;
    }
};

std::ostream& operator<<(std::ostream& os, const Position& pos)
{
    return os << "[" << pos.first << ", " << pos.second << "]";
}

Input parse_input(const char* path)
{
    auto lines = input_to_lines(path);
    Input input;
    input.reserve(lines.size());
    for (const auto& line : lines)
    {
        auto split_line = split(line, " ");
        Move move{line[0], uint8_t(std::stoi(split_line[1]))};
        input.push_back(move);
    }
    return input;
}

void move_knot(Position& pos, char direction)
{
    switch (direction)
    {
    case 'L':
        pos.first -= 1;
        return;
    case 'R':
        pos.first += 1;
        return;
    case 'D':
        pos.second -= 1;
        return;
    case 'U':
        pos.second += 1;
        return;
    }
}

bool adjust_pos(const Position& head_pos, const Position& tail_pos, Position& result_knot)
{
    int32_t first_diff = (head_pos.first - tail_pos.first) / 2;
    int32_t second_diff = (head_pos.second - tail_pos.second) / 2;
    if (first_diff == 0 && second_diff == 0)
    {
        return false;
    }
    result_knot.first = head_pos.first + (first_diff < 0 ? 1 : -1) * (first_diff != 0);
    result_knot.second = head_pos.second + (second_diff < 0 ? 1 : -1) * (second_diff != 0);
    return true;
}

uint64_t solve(const Input& input, size_t size)
{
    std::unordered_set<Position> tail_positions;
    std::vector<Position> knots(size, std::make_pair(0, 0));
    tail_positions.insert(knots[size - 1]);

    for (const auto& move : input)
    {
        for (uint8_t i = 0; i < move.amount; ++i)
        {
            move_knot(knots[0], move.direction);
            if (adjust_pos(knots[0], knots[1], knots[1]))
            {
                for (size_t i = 2; i < size; ++i)
                {
                    adjust_pos(knots[i - 1], knots[i], knots[i]);
                }
                tail_positions.insert(knots[size - 1]);
            }
        }
    }

    return tail_positions.size();
}

uint64_t step1(const Input& input)
{
    return solve(input, 2);
}

uint64_t step2(const Input& input)
{
    return solve(input, 10);
}

int main()
{
    for (size_t i = 0; i < sizeof(provided_paths) / sizeof(provided_paths[0]); ++i)
    {
        Input input = parse_input(provided_paths[i]);
        auto [res1, time1] = time_function(step1, input);
        std::cout << "==================\n";
        auto [res2, time2] = time_function(step2, input);

        std::cout << "Provided input #" << i << "\n";
        std::cout << "    Step1: Expected: " << provided_expected1[i] << "  |  Result: " << res1
                  << "  |  Time: " << time1 << "  |  " << (res1 == provided_expected1[i] ? "OK" : "KO") << "\n";
        std::cout << "    Step2: Expected: " << provided_expected2[i] << "  |  Result: " << res2
                  << "  |  Time: " << time2 << "  |  " << (res2 == provided_expected2[i] ? "OK" : "KO") << "\n";

        if (res1 != provided_expected1[i] || res2 != provided_expected2[i])
        {
            return 1;
        }
    }

    Input input = parse_input(real_input);
    auto [res1, time1] = time_function(step1, input);
    auto [res2, time2] = time_function(step2, input);
    std::cout << "\n=================================\n";
    std::cout << "Step1: Result: " << res1 << "  |  Time: " << time1 << " ms\n";
    std::cout << "Step2: Result: " << res2 << "  |  Time: " << time2 << " ms\n";
}
