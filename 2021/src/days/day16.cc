#include "common.hh"

using Bits = std::vector<bool>;

static std::array<bool, 4> hex_to_bin(char h)
{
    switch (h)
    {
    case '0':
        return {0, 0, 0, 0};
    case '1':
        return {0, 0, 0, 1};
    case '2':
        return {0, 0, 1, 0};
    case '3':
        return {0, 0, 1, 1};
    case '4':
        return {0, 1, 0, 0};
    case '5':
        return {0, 1, 0, 1};
    case '6':
        return {0, 1, 1, 0};
    case '7':
        return {0, 1, 1, 1};
    case '8':
        return {1, 0, 0, 0};
    case '9':
        return {1, 0, 0, 1};
    case 'A':
        return {1, 0, 1, 0};
    case 'B':
        return {1, 0, 1, 1};
    case 'C':
        return {1, 1, 0, 0};
    case 'D':
        return {1, 1, 0, 1};
    case 'E':
        return {1, 1, 1, 0};
    case 'F':
        return {1, 1, 1, 1};
    }
    return {0, 0, 0, 0};
}

static Bits parse_input(std::ifstream& input)
{
    std::string hex;
    std::getline(input, hex);
    Bits bits;
    bits.reserve(hex.size() * 4);

    for (char h : hex)
    {
        auto binary = hex_to_bin(h);
        bits.insert(bits.end(), binary.begin(), binary.end());
    }

    return bits;
}

struct Packet
{
    uint8_t version = 0;
    uint8_t type_id = 0;
    uint8_t length_type_id = 0;
    size_t data_index = 0;
    size_t data_length = 0;
    size_t n_subpackets = 0;

    uint64_t literal_value = 0;
    std::vector<Packet> sub_packets;

    void print(size_t depth = 0) const
    {
        std::cout << "[v:" << int(version) << ", t:" << int(type_id);
        if (type_id == 4)
        {
            std::cout << ", val:" << literal_value << "]\n";
        }
        else
        {
            std::cout << ", i:" << int(length_type_id);
            if (length_type_id == 0)
            {
                std::cout << ", len:" << data_length;
            }
            else
            {
                std::cout << ", subpackets:" << n_subpackets;
            }

            std::cout << "]\n";
            for (const auto& p : sub_packets)
            {
                for (size_t i = 0; i < depth; ++i)
                {
                    std::cout << "  ";
                }
                p.print(depth + 1);
            }
        }
    }
};

template <typename T>
static T parse_number(const Bits& bits, size_t& index, size_t n, T init = 0)
{
    T value = init;
    for (size_t i = 0; i < n; ++i)
    {
        value <<= 1;
        if (bits[index++])
        {
            value |= 1;
        }
    }
    return value;
}

static uint64_t parse_literal_value(const Bits& bits, size_t& index)
{
    uint64_t literal_value = 0;
    bool last = false;
    while (!last)
    {
        if (!bits[index++])
        {
            last = true;
        }
        literal_value = parse_number(bits, index, 4, literal_value);
    }
    return literal_value;
}

static Packet parse_packet(const Bits& bits, size_t& index)
{
    Packet packet;
    packet.version = parse_number<uint8_t>(bits, index, 3);
    packet.type_id = parse_number<uint8_t>(bits, index, 3);
    if (packet.type_id == 4)
    {
        packet.literal_value = parse_literal_value(bits, index);
    }
    else
    {
        packet.length_type_id = bits[index++];
        if (packet.length_type_id == 0)
        {
            packet.data_length = parse_number<size_t>(bits, index, 15);
            packet.data_index = index;
            while (index != packet.data_index + packet.data_length)
            {
                packet.sub_packets.push_back(parse_packet(bits, index));
            }
        }
        else
        {
            packet.n_subpackets = parse_number<size_t>(bits, index, 11);
            packet.data_index = index;
            for (size_t i = 0; i < packet.n_subpackets; ++i)
            {
                packet.sub_packets.push_back(parse_packet(bits, index));
            }
        }
    }
    return packet;
}

static std::vector<Packet> parse_packets(const Bits& bits)
{
    std::vector<Packet> packets;
    for (size_t i = 0; i < bits.size() - 6;)
    {
        packets.push_back(parse_packet(bits, i));
    }
    return packets;
}

static uint64_t step1_score(const std::vector<Packet>& packets)
{
    uint64_t score = 0;
    for (const auto& p : packets)
    {
        score += p.version + step1_score(p.sub_packets);
    }
    return score;
}

static uint64_t step1(const char* input_file)
{
    std::ifstream input(input_file);
    auto bits = parse_input(input);
    auto packets = parse_packets(bits);
    return step1_score(packets);
}

static uint64_t eval_packet(const Packet& packet);

static uint64_t eval_sum(const Packet& packet)
{
    uint64_t sum = 0;
    for (const auto& p : packet.sub_packets)
    {
        sum += eval_packet(p);
    }
    return sum;
}

static uint64_t eval_product(const Packet& packet)
{
    uint64_t product = 1;
    for (const auto& p : packet.sub_packets)
    {
        product *= eval_packet(p);
    }
    return product;
}

static uint64_t eval_min(const Packet& packet)
{
    uint64_t min = UINT64_MAX;
    for (const auto& p : packet.sub_packets)
    {
        uint64_t val = eval_packet(p);
        if (val < min)
        {
            min = val;
        }
    }
    return min;
}

static uint64_t eval_max(const Packet& packet)
{
    uint64_t max = 0;
    for (const auto& p : packet.sub_packets)
    {
        uint64_t val = eval_packet(p);
        if (val > max)
        {
            max = val;
        }
    }
    return max;
}

static uint64_t eval_greater(const Packet& packet)
{
    return eval_packet(packet.sub_packets[0]) > eval_packet(packet.sub_packets[1]);
}

static uint64_t eval_lower(const Packet& packet)
{
    return eval_packet(packet.sub_packets[0]) < eval_packet(packet.sub_packets[1]);
}

static uint64_t eval_equal(const Packet& packet)
{
    return eval_packet(packet.sub_packets[0]) == eval_packet(packet.sub_packets[1]);
}

static uint64_t eval_packet(const Packet& packet)
{
    switch (packet.type_id)
    {
    case 0:
        return eval_sum(packet);
    case 1:
        return eval_product(packet);
    case 2:
        return eval_min(packet);
    case 3:
        return eval_max(packet);
    case 4:
        return packet.literal_value;
    case 5:
        return eval_greater(packet);
    case 6:
        return eval_lower(packet);
    case 7:
        return eval_equal(packet);
    }
    return 0;
}

static uint64_t step2(const char* input_file)
{
    std::ifstream input(input_file);
    auto bits = parse_input(input);
    auto packets = parse_packets(bits);
    packets[0].print();
    return eval_packet(packets[0]);
}

void Day16::run()
{
    std::cout << "Day16:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;

    auto t0 = std::chrono::steady_clock::now();
    auto s1 = step1("../input/day16");
    auto t1 = std::chrono::steady_clock::now();
    auto s2 = step2("../input/day16");
    auto t2 = std::chrono::steady_clock::now();

    auto t_s1 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto t_s2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    std::cout << "    step1: " << s1 << " (" << t_s1 << " ms)\n";
    std::cout << "    step2: " << s2 << " (" << t_s2 << " ms)\n\n";
}

bool Day16::sanity_check()
{
    uint64_t step1_expected = 16;
    uint64_t step2_expected = 54;
    bool ret = true;

    uint64_t step1_got = step1("../input/day16_provided");
    if (step1_got != step1_expected)
    {
        fprintf(stderr, "\n    step1 sanity check failed: expected %llu, got %llu\n", step1_expected, step1_got);
        ret = false;
    }

    uint64_t step2_got = step2("../input/day16_provided2");
    if (step2_got != step2_expected)
    {
        fprintf(stderr, "\n    step2 sanity check failed: expected %llu, got %llu\n", step2_expected, step2_got);
        ret = false;
    }

    if (ret)
        std::cout << " OK\n";
    return ret;
}
