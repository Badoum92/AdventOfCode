#include "common.h"

const char* provided_paths[] = {"input/day13_provided"};
uint64_t provided_expected1[] = {13};
uint64_t provided_expected2[] = {140};

const char* real_input = "input/day13";

struct Packet
{
    uint32_t value = 0;
    std::vector<Packet> list;
    bool is_list = false;
    bool is_divider = false;
};

std::ostream& operator<<(std::ostream& os, const Packet& packet)
{
    if (packet.is_list)
    {
        os << "[";
        if (packet.list.size() > 0)
        {
            os << packet.list[0];
        }
        for (size_t i = 1; i < packet.list.size(); ++i)
        {
            os << "," << packet.list[i];
        }
        os << "]";
    }
    else
    {
        os << packet.value;
    }
    return os;
}
void print_packet(const Packet& packet)
{
    if (packet.is_list)
    {
        std::cout << "[";
        if (packet.list.size() > 0)
        {
            print_packet(packet.list[0]);
        }
        for (size_t i = 1; i < packet.list.size(); ++i)
        {
            std::cout << ", ";
            print_packet(packet.list[i]);
        }
        std::cout << "]";
    }
    else
    {
        std::cout << packet.value;
    }
}

using Input = std::vector<Packet>;

Packet parse_number(const std::vector<std::string>& tokens, size_t& i)
{
    Packet e;
    e.is_list = false;
    e.value = std::stoul(tokens[i]);
    i++;
    return e;
}

Packet parse_list(const std::vector<std::string>& tokens, size_t& i)
{
    Packet e;
    e.is_list = true;
    while (tokens[i] != "]")
    {
        if (tokens[i] == ",")
        {
            i++;
        }
        else if (tokens[i] == "[")
        {
            i++;
            e.list.push_back(parse_list(tokens, i));
            i++;
        }
        else
        {
            e.list.push_back(parse_number(tokens, i));
        }
    }
    return e;
}

Packet parse_line(const std::string& line)
{
    auto tokens = split(line, " ");
    size_t i = 1;
    return parse_list(tokens, i);
}

Input parse_input(const char* path)
{
    Input input;
    auto lines = input_to_lines(path);
    for (size_t i = 0; i < lines.size(); i += 3)
    {
        input.push_back(parse_line(lines[i]));
        input.push_back(parse_line(lines[i + 1]));
    }
    return input;
}

int compare_numbers(uint32_t a, uint32_t b)
{
    if (a < b)
    {
        return -1;
    }
    else if (a == b)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int compare_packets(const Packet& a, const Packet& b);

int compare_list(const std::vector<Packet>& a, const std::vector<Packet>& b)
{
    for (size_t i = 0; i < a.size() && i < b.size(); ++i)
    {
        int cmp = compare_packets(a[i], b[i]);
        if (cmp != 0)
        {
            return cmp;
        }
    }
    if (a.size() < b.size())
    {
        return -1;
    }
    else if (a.size() == b.size())
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int compare_packets(const Packet& a, const Packet& b)
{
    if (a.is_list && b.is_list)
    {
        return compare_list(a.list, b.list);
    }
    else if (!a.is_list && !b.is_list)
    {
        return compare_numbers(a.value, b.value);
    }
    else if (a.is_list && !b.is_list)
    {
        std::vector<Packet> fake_list;
        fake_list.push_back(b);
        return compare_list(a.list, fake_list);
    }
    else
    {
        std::vector<Packet> fake_list;
        fake_list.push_back(a);
        return compare_list(fake_list, b.list);
    }
}

bool less(const Packet& a, const Packet& b)
{
    return compare_packets(a, b) != 1;
}

uint64_t step1(const Input& input)
{
    uint64_t total = 0;
    for (size_t i = 0; i < input.size(); i += 2)
    {
        if (less(input[i], input[i + 1]))
        {
            total += (i / 2) + 1;
        }
    }
    return total;
}

uint64_t step2(Input input)
{
    input.push_back(parse_line("[ [ 2 ] ]"));
    input.back().is_divider = true;
    input.push_back(parse_line("[ [ 6 ] ]"));
    input.back().is_divider = true;

    std::sort(input.begin(), input.end(), less);

    uint64_t result = 1;
    for (size_t i = 0; i < input.size(); ++i)
    {
        if (input[i].is_divider)
        {
            result *= i + 1;
        }
    }
    return result;
}

int main()
{
    for (size_t i = 0; i < sizeof(provided_paths) / sizeof(provided_paths[0]); ++i)
    {
        Input input = parse_input(provided_paths[i]);
        auto [res1, time1] = time_function(step1, input);
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
