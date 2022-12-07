#include "common.h"

const char* provided_paths[] = {"input/day7_provided"};
uint64_t provided_expected1[] = {95437};
uint64_t provided_expected2[] = {24933642};

const char* real_input = "input/day7";

struct File
{
    File(const std::vector<std::string>& file_desc)
        : name(file_desc[1])
        , size(std::stoull(file_desc[0]))
    {}

    std::string name;
    uint64_t size;
};

struct Directory
{
    Directory(const std::vector<std::string>& dir_desc, Directory* parent_ = nullptr)
        : name(dir_desc[1])
        , size(0)
        , parent(parent_)
    {}

    uint64_t get_size() const
    {
        return size;
    }

    uint64_t get_size()
    {
        if (size != 0)
        {
            return size;
        }

        for (const auto& file : files)
        {
            size += file.size;
        }
        for (auto& dir : dirs)
        {
            size += dir.get_size();
        }
        return size;
    }

    std::string name;
    std::vector<File> files;
    std::vector<Directory> dirs;
    uint64_t size;
    Directory* parent;
};

using Input = Directory;
using SplitLines = std::vector<std::vector<std::string>>;

size_t ls_cmd(Directory* dir, const SplitLines& input, size_t i)
{
    for (; i < input.size() && input[i][0] != "$"; ++i)
    {
        const auto& line = input[i];
        if (line[0] == "dir")
        {
            dir->dirs.emplace_back(line, dir);
        }
        else
        {
            dir->files.emplace_back(line);
        }
    }
    return i;
}

size_t cd_cmd(Directory*& dir, const SplitLines& input, size_t i)
{
    const auto& line = input[i];
    if (line[2] == "..")
    {
        dir = dir->parent;
    }
    else
    {
        for (auto& d : dir->dirs)
        {
            if (d.name == line[2])
            {
                dir = &d;
                break;
            }
        }
    }
    return i + 1;
}

Input parse_input(const char* path)
{
    auto lines = input_to_lines(path);
    SplitLines split_lines;
    split_lines.reserve(lines.size());
    for (const auto& line : lines)
    {
        split_lines.push_back(split(line, " "));
    }

    Directory root({"dir", "/"});
    Directory* dir = &root;

    for (size_t i = 1; i < split_lines.size();)
    {
        const auto& line = split_lines[i];
        if (line[0] == "$")
        {
            if (line[1] == "ls")
            {
                i = ls_cmd(dir, split_lines, i + 1);
                continue;
            }
            else
            {
                i = cd_cmd(dir, split_lines, i);
                continue;
            }
        }
    }
    root.get_size();
    return root;
}

void sum_step1(const Directory* dir, uint64_t& size)
{
    if (dir->get_size() <= 100000)
    {
        size += dir->get_size();
    }

    for (const auto& d : dir->dirs)
    {
        sum_step1(&d, size);
    }
}

uint64_t step1(const Input& input)
{
    uint64_t size = 0;
    sum_step1(&input, size);
    return size;
}

uint64_t find_step2(const Directory* dir, uint64_t need_delete)
{
    uint64_t size = dir->get_size();
    for (const auto& d : dir->dirs)
    {
        uint64_t new_size = find_step2(&d, need_delete);
        if (new_size > need_delete && new_size < size)
        {
            size = new_size;
        }
    }
    return size;
}

uint64_t step2(const Input& input)
{
    uint64_t free_space = 70000000 - input.get_size();
    uint64_t needed_space = 30000000;
    uint64_t need_delete = needed_space - free_space;
    return find_step2(&input, need_delete);
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
