#include "common.hh"

using passport_t = std::unordered_map<std::string, std::string>;

static passport_t parse_passport(std::ifstream& input)
{
    passport_t passport;
    std::string s;
    while (std::getline(input, s))
    {
        if (s.empty())
            break;
        auto fields = split(s, " ");
        for (const auto& field : fields)
        {
            auto key_val = split(field, ":");
            passport[key_val[0]] = key_val[1];
        }
    }
    return passport;
}

static unsigned step1(const char* input_file)
{
    std::ifstream input(input_file);
    std::vector<passport_t> passports;
    while (!input.eof())
        passports.push_back(parse_passport(input));

    unsigned total = 0;
    for (const auto& p : passports)
    {
        if (p.size() == 8)
        {
            total++;
        }
        else if (p.size() == 7)
        {
            if (p.count("cid") == 0)
                total++;
        }
    }

    return total;
}

static bool check_byr(const std::string& val)
{
    int year = std::stoi(val);
    if (year < 1920 || 2002 < year)
    {
        return false;
    }
    return true;
}

static bool check_iyr(const std::string& val)
{
    int year = std::stoi(val);
    if (year < 2010 || 2020 < year)
    {
        return false;
    }
    return true;
}

static bool check_eyr(const std::string& val)
{
    int year = std::stoi(val);
    if (year < 2020 || 2030 < year)
    {
        return false;
    }
    return true;
}

static bool check_hgt(const std::string& val)
{
    int height = 0;
    std::string unit;
    unit.resize(2);
    sscanf(val.c_str(), "%d%s", &height, unit.data());
    if (unit == "cm")
    {
        if (height < 150 || 193 < height)
        {
            return false;
        }
    }
    else if (unit == "in")
    {
        if (height < 59 || 76 < height)
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

static bool check_hcl(const std::string& val)
{
    if (val[0] != '#')
    {
        return false;
    }
    bool bad = false;
    for (unsigned i = 1; i < val.size(); ++i)
    {
        if (i > 6
            || ((val[i] < '0' || '9' < val[i])
                && (val[i] < 'a' || 'f' < val[i])))
            bad = true;
    }
    if (bad)
    {
        return false;
    }
    return true;
}

static bool check_ecl(const std::string& val)
{
    static std::array<std::string, 7> valid = {"amb", "blu", "brn", "gry",
                                               "grn", "hzl", "oth"};
    if (std::find(begin(valid), end(valid), val) == end(valid))
    {
        return false;
    }
    return true;
}

static bool check_pid(const std::string& val)
{
    if (val.size() != 9)
    {
        return false;
    }
    int id = std::stoi(val);
    if (id == 0)
    {
        return false;
    }
    return true;
}

static unsigned step2(const char* input_file)
{
    std::ifstream input(input_file);
    std::vector<passport_t> passports;
    while (!input.eof())
        passports.push_back(parse_passport(input));

    unsigned total = 0;
    for (const auto& p : passports)
    {
        if (p.size() < 7)
        {
            continue;
        }
        else if (p.size() == 7)
        {
            if (p.count("cid") != 0)
                continue;
        }

        bool valid_passport = true;
        for (const auto& [key, val] : p)
        {
            if (key == "byr")
                valid_passport = check_byr(val);
            else if (key == "iyr")
                valid_passport = check_iyr(val);
            else if (key == "eyr")
                valid_passport = check_eyr(val);
            else if (key == "hgt")
                valid_passport = check_hgt(val);
            else if (key == "hcl")
                valid_passport = check_hcl(val);
            else if (key == "ecl")
                valid_passport = check_ecl(val);
            else if (key == "pid")
                valid_passport = check_pid(val);

            if (!valid_passport)
                break;
        }

        if (valid_passport)
            total++;
    }

    return total;
}

void Day4::run()
{
    std::cout << "Day4:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day4::sanity_check()
{
    unsigned step1_expected = 2;
    unsigned step2_expected = 4;
    bool ret = true;

    unsigned step1_got = step1(provided_file);
    if (step1_got != step1_expected)
    {
        fprintf(stderr,
                "\n    step1 sanity check failed: expected %u, got %u\n",
                step1_expected, step1_got);
        ret = false;
    }

    unsigned step2_got =
        step2((std::string(provided_file) + std::string("2")).c_str());
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
