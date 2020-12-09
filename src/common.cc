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
    size_t idx = 0;
    while (idx != s.npos)
    {
        idx = s.find(delim, cur);
        std::string sub = s.substr(cur, idx - cur);
        if (!sub.empty())
            v.push_back(sub);
        cur = idx + delim.size();
    }
    return v;
}
