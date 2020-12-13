#include "common.hh"

struct Ship
{
    int x = 0;
    int y = 0;
    int direction = 0;
    // step2
    int wp_x = 10;
    int wp_y = 1;
};

char directions[] = {'E', 'N', 'W', 'S'};

template<typename F>
unsigned solve(F function, const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);
    Ship s;
    for (const auto& l : lines)
    {
        char c;
        int val;
        if (sscanf(l.c_str(), "%c%d", &c, &val) != 2)
        {
            std::cerr << "wtf (sscanf)\n";
            exit(2);
        }
        function(c, val, s);
    }
    return std::abs(s.x) + std::abs(s.y);
}

void apply_instruction(char c, int val, Ship& s)
{
    switch (c)
    {
    case 'N':
        s.y += val;
        return;
    case 'S':
        s.y -= val;
        return;
    case 'E':
        s.x += val;
        return;
    case 'W':
        s.x -= val;
        return;
    case 'L':
        s.direction = ((s.direction + (val / 90)) % 4 + 4) % 4;
        return;
    case 'R':
        apply_instruction('L', -val, s);
        return;
    case 'F':
        apply_instruction(directions[s.direction], val, s);
        return;
    default:
        std::cerr << "wtf (apply_instruction)\n";
        exit(2);
    }
}

static unsigned step1(const char* input_file)
{
    return solve(apply_instruction, input_file);
}

void apply_instruction2(char c, int val, Ship& s)
{
    int x = s.wp_x;
    int y = s.wp_y;
    double angle = (val * 3.1415926535) / 180;
    switch (c)
    {
    case 'N':
        s.wp_y += val;
        return;
    case 'S':
        s.wp_y -= val;
        return;
    case 'E':
        s.wp_x += val;
        return;
    case 'W':
        s.wp_x -= val;
        return;
    case 'L':
        s.wp_x = std::round(x * std::cos(angle) - y * std::sin(angle));
        s.wp_y = std::round(x * std::sin(angle) + y * std::cos(angle));
        return;
    case 'R':
        apply_instruction2('L', -val, s);
        return;
    case 'F':
        s.x += val * s.wp_x;
        s.y += val * s.wp_y;
        return;
    default:
        std::cerr << "wtf (apply_instruction2)\n";
        exit(2);
    }
}

static unsigned step2(const char* input_file)
{
    return solve(apply_instruction2, input_file);
}

void Day12::run()
{
    std::cout << "Day12:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day12::sanity_check()
{
    unsigned step1_expected = 25;
    unsigned step2_expected = 286;
    bool ret = true;

    unsigned step1_got = step1(provided_file);
    if (step1_got != step1_expected)
    {
        fprintf(stderr,
                "\n    step1 sanity check failed: expected %u, got %u\n",
                step1_expected, step1_got);
        ret = false;
    }

    unsigned step2_got = step2(provided_file);
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
