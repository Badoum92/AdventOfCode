#include "common.hh"

using Ticket = std::vector<unsigned>;

struct Rule
{
    Rule(const char* n, unsigned l1, unsigned h1, unsigned l2, unsigned h2)
        : name(n)
        , range1(l1, h1)
        , range2(l2, h2)
    {}

    bool contains(unsigned x) const
    {
        return (range1.first <= x && x <= range1.second)
            || (range2.first <= x && x <= range2.second);
    }

    std::string name;
    std::pair<unsigned, unsigned> range1;
    std::pair<unsigned, unsigned> range2;
};

static std::tuple<std::vector<Rule>, Ticket, std::vector<Ticket>>
parse_input(const std::vector<std::string>& lines)
{
    std::vector<Rule> rules;
    Ticket my_ticket;
    std::vector<Ticket> nearby_tickets;

    unsigned i = 0;

    char rule_name[32];
    unsigned l1, h1, l2, h2;

    for (; lines[i] != ""; ++i)
    {
        sscanf(lines[i].c_str(), "%[^:]: %d-%d or %d-%d", rule_name, &l1, &h1,
               &l2, &h2);
        rules.emplace_back(rule_name, l1, h1, l2, h2);
    }
    i += 2;

    auto split_str = split(lines[i], ",");
    for (const auto& str : split_str)
        my_ticket.push_back(std::stoul(str));
    i += 3;

    for (; i < lines.size(); ++i)
    {
        split_str = split(lines[i], ",");
        Ticket t;
        for (const auto& str : split_str)
            t.push_back(std::stoul(str));
        nearby_tickets.push_back(std::move(t));
    }

    return std::make_tuple(rules, my_ticket, nearby_tickets);
}

unsigned solve_step1(const std::vector<Rule>& rules,
                     std::vector<Ticket>& nearby_tickets)
{
    unsigned total = 0;
    for (auto it = begin(nearby_tickets); it != end(nearby_tickets);)
    {
        bool match = false;
        for (const auto& e : *it)
        {
            match = std::any_of(begin(rules), end(rules), [&](const Rule& r) {
                return r.contains(e);
            });
            if (!match)
            {
                total += e;
                it = nearby_tickets.erase(it);
                break;
            }
        }
        if (match)
            ++it;
    }
    return total;
}

static unsigned step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);
    auto [rules, my_ticket, nearby_tickets] = parse_input(lines);
    return solve_step1(rules, nearby_tickets);
}

static uint64_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);
    auto [rules, my_ticket, nearby_tickets] = parse_input(lines);
    solve_step1(rules, nearby_tickets);

    std::vector<std::unordered_set<const Rule*>> cols_to_rules(
        my_ticket.size());
    std::vector<const Rule*> rules_association(my_ticket.size());
    std::vector<std::vector<unsigned>> cols(my_ticket.size());

    for (unsigned i = 0; i < my_ticket.size(); ++i)
    {
        for (const auto& t : nearby_tickets)
        {
            cols[i].push_back(t[i]);
        }
    }

    for (unsigned i = 0; i < cols.size(); ++i)
    {
        for (const auto& r : rules)
        {
            if (std::all_of(begin(cols[i]), end(cols[i]),
                            [&](unsigned x) { return r.contains(x); }))
            {
                cols_to_rules[i].insert(&r);
            }
        }
    }

    for (unsigned i = 0; i < cols_to_rules.size();)
    {
        if (cols_to_rules[i].size() != 1)
        {
            ++i;
            continue;
        }

        const Rule* r = *begin(cols_to_rules[i]);
        rules_association[i] = r;

        for (auto& set : cols_to_rules)
        {
            set.erase(r);
        }

        i = 0;
    }

    uint64_t total = 1;
    for (unsigned i = 0; i < rules_association.size(); ++i)
    {
        const auto& r = rules_association[i];
        if (r->name.find("departure") == 0)
        {
            total *= my_ticket[i];
        }
    }
    return total;
}

void Day16::run()
{
    std::cout << "Day16:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day16::sanity_check()
{
    unsigned step1_expected = 71;
    bool ret = true;

    unsigned step1_got = step1(provided_file);
    if (step1_got != step1_expected)
    {
        fprintf(stderr,
                "\n    step1 sanity check failed: expected %u, got %u\n",
                step1_expected, step1_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
