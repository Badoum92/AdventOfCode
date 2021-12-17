#include "common.hh"

struct Point
{
    int32_t x;
    int32_t y;
};

struct Box
{
    Point min;
    Point max;
};

struct State
{
    State(const Point& v)
        : velocity(v)
    {}

    Point pos = {0, 0};
    Point velocity;
    int32_t max_y = 0;
};

static Box parse_input(std::ifstream& input)
{
    const auto tokens = input_to_tokens<int32_t>(input);
    Box b;
    b.min = {tokens[0], tokens[2]};
    b.max = {tokens[1], tokens[3]};
    return b;
}

static bool point_in_box(const Point& p, const Box& b)
{
    return (b.min.x <= p.x && p.x <= b.max.x) && (b.min.y <= p.y && p.y <= b.max.y);
}

static bool simulate(State& state, const Box& target)
{
    while (state.pos.x < target.max.x && state.pos.y > target.min.y)
    {
        state.pos.x += state.velocity.x;
        state.pos.y += state.velocity.y;

        if (state.pos.y > state.max_y)
        {
            state.max_y = state.pos.y;
        }

        if (point_in_box(state.pos, target))
        {
            return true;
        }

        state.velocity.y--;
        if (state.velocity.x > 0)
        {
            state.velocity.x--;
        }
        else if (state.velocity.x < 0)
        {
            state.velocity.x++;
        }
    }
    return false;
}

static std::pair<int32_t, int32_t> brute_force(const Box& target)
{
    int32_t total = 0;
    int32_t max_y = 0;
    for (int32_t y = target.min.y; y <= -target.min.y; ++y)
    {
        for (int32_t x = 0; x <= target.max.x; ++x)
        {
            State state({x, y});
            if (simulate(state, target))
            {
                total++;
                if (state.max_y > max_y)
                {
                    max_y = state.max_y;
                }
            }
        }
    }
    return std::make_pair(total, max_y);
}

static int32_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto target = parse_input(input);
    auto [total, max_y] = brute_force(target);
    return max_y;
}

static int32_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto target = parse_input(input);
    auto [total, max_y] = brute_force(target);
    return total;
}

void Day17::run()
{
    std::cout << "Day17:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;

    auto t0 = std::chrono::steady_clock::now();
    auto s1 = step1("../input/day17");
    auto t1 = std::chrono::steady_clock::now();
    auto s2 = step2("../input/day17");
    auto t2 = std::chrono::steady_clock::now();

    auto t_s1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto t_s2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "    step1: " << s1 << " (" << t_s1 << " ms)\n";
    std::cout << "    step2: " << s2 << " (" << t_s2 << " ms)\n\n";
}

bool Day17::sanity_check()
{
    int32_t step1_expected = 45;
    int32_t step2_expected = 112;
    bool ret = true;

    int32_t step1_got = step1("../input/day17_provided");
    if (step1_got != step1_expected)
    {
        fprintf(stderr, "\n    step1 sanity check failed: expected %d, got %d\n", step1_expected, step1_got);
        ret = false;
    }

    int32_t step2_got = step2("../input/day17_provided");
    if (step2_got != step2_expected)
    {
        fprintf(stderr, "\n    step2 sanity check failed: expected %d, got %d\n", step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
