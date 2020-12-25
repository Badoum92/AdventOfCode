#include "common.hh"

#include <list>

template <typename T>
static int wrap_index(const T& cups, int i)
{
    if (i < 0)
        return i + cups.size();
    else if (i >= (int)cups.size())
        return i % cups.size();
    return i;
}

template <typename T>
static int get_next_cup_idx(const T& cups, int cur_idx)
{
    for (int i = 1;; ++i)
    {
        char to_find = cups[cur_idx] - i;
        if (to_find <= 0)
            to_find += 9;

        for (int j = wrap_index(cups, cur_idx + 4); j != cur_idx;
             j = wrap_index(cups, j + 1))
        {
            if (cups[j] == to_find)
                return j;
        }
    }
}

static std::string step1(const char* input_file)
{
    std::ifstream input(input_file);
    std::string cups;
    input >> cups;

    for (int cur = 0; cur < 100; ++cur)
    {
        int cup_idx = cur % cups.size();
        int next_cup_idx = get_next_cup_idx(cups, cup_idx);

        std::array<char, 3> range;
        range[0] = cups[wrap_index(cups, cup_idx + 1)];
        range[1] = cups[wrap_index(cups, cup_idx + 2)];
        range[2] = cups[wrap_index(cups, cup_idx + 3)];

        int i = wrap_index(cups, cup_idx + 4);
        for (; i != next_cup_idx; i = wrap_index(cups, i + 1))
        {
            cups[wrap_index(cups, i - 3)] = cups[i];
        }
        cups[wrap_index(cups, i - 3)] = cups[i];

        for (int j = 0; j < 3; ++j)
        {
            cups[wrap_index(cups, next_cup_idx - 2 + j)] = range[j];
        }
    }

    std::string res;
    int one_index = cups.find('1');
    for (unsigned i = 1; i < 9; ++i)
    {
        res += cups[wrap_index(cups, one_index + i)];
    }
    return res;
}

static std::vector<unsigned> parse_input(const std::string& str)
{
    std::vector<unsigned> cups(1000000, 0);
    std::vector<unsigned> links(1000001, 0);
    for (unsigned i = 0; i < str.size(); ++i)
    {
        cups[i] = str[i] - '0';
    }
    for (unsigned i = str.size(); i < cups.size(); ++i)
    {
        cups[i] = i + 1;
    }

    for (unsigned i = 0; i < cups.size() - 1; ++i)
    {
        links[cups[i]] = cups[i + 1];
    }
    links[1000000] = cups[0];

    return links;
}

static unsigned step(std::vector<unsigned>& links, unsigned cur)
{
    unsigned a = links[cur];
    unsigned b = links[a];
    unsigned c = links[b];

    unsigned next = cur - 1;
    if (next == 0)
        next = 1000000;

    while (next == a || next == b || next == c)
    {
        next = next - 1;
        if (next == 0)
            next = 1000000;
    }

    links[cur] = links[c];
    links[c] = links[next];
    links[next] = a;

    return links[cur];
}

static uint64_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    std::string str;
    input >> str;
    auto links = parse_input(str);

    unsigned cur = str[0] - '0';
    for (int i = 0; i < 10000000; ++i)
    {
        cur = step(links, cur);
    }

    uint64_t a = links[1];
    uint64_t b = links[a];
    return a * b;
}

void Day23::run()
{
    std::cout << "Day23:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day23::sanity_check()
{
    std::string step1_expected = "67384529";
    uint64_t step2_expected = 149245887792ULL;
    bool ret = true;

    std::string step1_got = step1(provided_file);
    if (step1_got != step1_expected)
    {
        fprintf(stderr,
                "\n    step1 sanity check failed: expected %s, got %s\n",
                step1_expected.c_str(), step1_got.c_str());
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
