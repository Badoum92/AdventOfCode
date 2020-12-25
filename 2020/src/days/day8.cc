#include "common.hh"

struct Instruction
{
    std::string opcode;
    int arg;
};

static std::pair<int, bool> exec_instructions(std::vector<Instruction>& instructions, int acc, unsigned i, std::unordered_set<unsigned> set)
{
    while (i < instructions.size())
    {
        auto& instr = instructions[i];

        if (set.count(i))
        {
            break;
        }

        set.insert(i);

        if (instr.opcode == "acc")
        {
            acc += instr.arg;
        }
        else if (instr.opcode == "jmp")
        {
            i += instr.arg;
            continue;
        }

        ++i;
    }
    return {acc, i == instructions.size()};
}

static int step1(const char* input_file)
{
    std::ifstream input(input_file);
    std::vector<Instruction> instructions;
    while (!input.eof())
    {
        Instruction instr;
        input >> instr.opcode >> instr.arg;
        instructions.push_back(instr);
    }

    return exec_instructions(instructions, 0, 0, std::unordered_set<unsigned>()).first;
}

static int step2(const char* input_file)
{
    std::ifstream input(input_file);
    std::vector<Instruction> instructions;
    while (!input.eof())
    {
        Instruction instr;
        input >> instr.opcode >> instr.arg;
        instructions.push_back(instr);
    }

    std::unordered_set<unsigned> set;
    int acc = 0;
    unsigned i = 0;
    while (i < instructions.size())
    {
        auto& instr = instructions[i];

        if (set.count(i))
        {
            break;
        }

        set.insert(i);

        if (instr.opcode == "acc")
        {
            acc += instr.arg;
        }
        else if (instr.opcode == "jmp")
        {
            instructions[i].opcode = "nop";
            auto [acc_clone, success] = exec_instructions(instructions, acc, i + 1, set);
            if (success)
                return acc_clone;
            instructions[i].opcode = "jmp";

            i += instr.arg;
            continue;
        }
        else if (instr.opcode == "nop")
        {
            instructions[i].opcode = "jmp";
            auto [acc_clone, success] = exec_instructions(instructions, acc, i + instr.arg, set);
            if (success)
                return acc_clone;
            instructions[i].opcode = "nop";
        }

        ++i;
    }

    return 0;
}

void Day8::run()
{
    std::cout << "Day8:\n";
    std::cout << "    running sanity check...";
    if (!sanity_check())
        return;
    std::cout << "    step1: " << step1(input_file) << "\n";
    std::cout << "    step2: " << step2(input_file) << "\n\n";
}

bool Day8::sanity_check()
{
    int step1_expected = 5;
    int step2_expected = 8;
    bool ret = true;

    int step1_got = step1(provided_file);
    if (step1_got != step1_expected)
    {
        fprintf(stderr,
                "\n    step1 sanity check failed: expected %u, got %u\n",
                step1_expected, step1_got);
        ret = false;
    }

    int step2_got = step2(provided_file);
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
