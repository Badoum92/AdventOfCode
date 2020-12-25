#include "common.hh"

static uint64_t eval(uint64_t a, char op, uint64_t b)
{
    if (op == '*')
        return a * b;
    return a + b;
}

static void op_to_res(std::stack<uint64_t>& res_stack,
                      std::stack<char>& op_stack)
{
    uint64_t a = res_stack.top();
    res_stack.pop();
    uint64_t b = res_stack.top();
    res_stack.pop();
    res_stack.push(eval(a, op_stack.top(), b));
    op_stack.pop();
}

static bool prec_func1(char, char)
{
    return true;
}

static bool prec_func2(char a, char b)
{
    if (a == '+')
        return true;
    return b == '*';
}

template <typename F>
static uint64_t compute_expression(const std::string& line, F prec_func)
{
    std::stack<char> op_stack;
    std::stack<uint64_t> res_stack;
    for (unsigned i = 0; i < line.size(); ++i)
    {
        char c = line[i];
        if ('0' <= c && c <= '9')
        {
            res_stack.push(c - '0');
        }
        else if (c == '*' || c == '+')
        {
            while (!op_stack.empty()
                   && prec_func(op_stack.top(), c)
                   && op_stack.top() != '(')
            {
                op_to_res(res_stack, op_stack);
            }
            op_stack.push(c);
        }
        else if (c == '(')
        {
            op_stack.push(c);
        }
        else if (c == ')')
        {
            while (op_stack.top() != '(')
            {
                op_to_res(res_stack, op_stack);
            }
            if (op_stack.top() == '(')
            {
                op_stack.pop();
            }
        }
    }
    while (!op_stack.empty())
        op_to_res(res_stack, op_stack);
    return res_stack.top();
}

static uint64_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);
    uint64_t total = 0;
    for (const auto& l : lines)
    {
        total += compute_expression(l, prec_func1);
    }
    return total;
}

static uint64_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);
    uint64_t total = 0;
    for (const auto& l : lines)
    {
        total += compute_expression(l, prec_func2);
    }
    return total;
}

void Day18::run()
{
    std::cout << "Day18:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day18::sanity_check()
{
    uint64_t step1_expected = 13632;
    uint64_t step2_expected = 23340;
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
