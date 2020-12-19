#include "common.hh"

struct Rule
{
    void print() const
    {
        if (match)
        {
            std::cout << '"' << *match << "\"";
        }
        else
        {
            for (const auto& sub : *subrules)
            {
                for (const auto& e : sub)
                {
                    std::cout << e << " ";
                }
                std::cout << "| ";
            }
        }
    }

    std::optional<char> match;
    std::optional<std::vector<std::vector<unsigned>>> subrules;
};

static unsigned parse_rules(std::vector<Rule>& rules,
                            const std::vector<std::string>& lines)
{
    for (unsigned line_idx = 0; line_idx < lines.size(); ++line_idx)
    {
        if (lines[line_idx] == "")
        {
            ++line_idx;
            return line_idx;
        }

        auto parts = split(lines[line_idx], " ");
        unsigned rule_nb = std::stoul(parts[0]);
        std::vector<std::vector<unsigned>> rule;
        std::vector<unsigned> subrule;
        for (unsigned j = 1; j < parts.size(); ++j)
        {
            if (parts[j][0] == '"')
            {
                rules[rule_nb].match = parts[j][1];
                break;
            }
            else if (parts[j] == "|")
            {
                rule.push_back(std::move(subrule));
                subrule.clear();
            }
            else
            {
                subrule.push_back(std::stoul(parts[j]));
            }
        }
        if (!rule.empty() || !subrule.empty())
        {
            rule.push_back(std::move(subrule));
            rules[rule_nb].subrules = rule;
        }
    }
    return 0;
}

static std::optional<unsigned>
match(const char* msg, const std::vector<Rule>& rules, unsigned rule_index)
{
    const auto& rule = rules[rule_index];
    if (rule.match)
    {
        return *msg == *rule.match ? std::optional<unsigned>(1) : std::nullopt;
    }
    for (const auto& sub : *rule.subrules)
    {
        bool matched = true;
        unsigned offset = 0;
        for (const auto& r : sub)
        {
            auto opt = match(msg + offset, rules, r);
            matched = opt.has_value();
            if (!matched)
                break;
            offset += *opt;
        }
        if (matched)
            return offset;
    }
    return std::nullopt;
}

static unsigned step1(const char* input_file)
{
    std::vector<Rule> rules(131);
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);
    unsigned line_idx = parse_rules(rules, lines);

    unsigned total = 0;
    for (; line_idx < lines.size(); ++line_idx)
    {
        auto offset = match(lines[line_idx].c_str(), rules, 0);
        if (offset && *offset == lines[line_idx].size())
        {
            ++total;
        }
    }
    return total;
}

static std::vector<unsigned>
match2(const char* msg, const std::vector<Rule>& rules, unsigned rule_index)
{
    if (*msg == 0)
        return std::vector<unsigned>();

    const auto& rule = rules[rule_index];
    if (rule.match)
    {
        if (*msg == *rule.match)
        {
            return std::vector<unsigned>(1, 1);
        }
        else
        {
            return std::vector<unsigned>();
        }
    }
    std::vector<unsigned> ret_offsets;
    for (const auto& sub : *rule.subrules)
    {
        std::vector<unsigned> offsets(1, 0);
        for (const auto& r : sub)
        {
            std::vector<unsigned> new_offsets;
            for (auto& off : offsets)
            {
                auto ret = match2(msg + off, rules, r);
                for (const auto& o : ret)
                {
                    new_offsets.push_back(off + o);
                }
            }
            std::swap(offsets, new_offsets);
        }
        ret_offsets.insert(end(ret_offsets), begin(offsets), end(offsets));
    }
    return ret_offsets;
}

static unsigned step2(const char* input_file)
{
    std::vector<Rule> rules(131);
    std::ifstream input(input_file);
    auto lines = input_to_lines(input);
    unsigned line_idx = parse_rules(rules, lines);

    unsigned total = 0;
    for (; line_idx < lines.size(); ++line_idx)
    {
        auto offsets = match2(lines[line_idx].c_str(), rules, 0);
        for (const auto& off : offsets)
        {
            if (off == lines[line_idx].size())
            {
                ++total;
                break;
            }
        }
    }
    return total;
}

void Day19::run()
{
    std::cout << "Day19:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    // std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day19::sanity_check()
{
    // unsigned step1_expected = 2;
    unsigned step2_expected = 12;
    bool ret = true;

    /*unsigned step1_got = step1(provided_file);
    if (step1_got != step1_expected)
    {
        fprintf(stderr,
                "\n    step1 sanity check failed: expected %u, got %u\n",
                step1_expected, step1_got);
        ret = false;
    }*/

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
