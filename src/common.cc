#include "common.hh"

std::vector<std::string> input_to_lines(std::ifstream& input)
{
    std::vector<std::string> v;
    std::string s;
    while (std::getline(input, s))
    {
        v.push_back(s);
    }
    return v;
}

std::vector<std::string> split(const std::string& s, const std::string& delim)
{
    std::vector<std::string> v;
    size_t cur = 0;
    while (true)
    {
        size_t idx = s.find(delim, cur);
        std::string sub = s.substr(cur, idx - cur);
        if (!sub.empty())
            v.push_back(std::move(sub));
        cur = idx + delim.size();
        if (idx == s.npos)
            break;
    }
    return v;
}
