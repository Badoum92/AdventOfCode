#include "common.hh"

int main(int argc, char** argv)
{
    // clang-format off
    std::unique_ptr<Day> days[] = {
        std::make_unique<Day1>(),
        // std::make_unique<Day2>(),
        // std::make_unique<Day3>(),
        // std::make_unique<Day4>(),
        // std::make_unique<Day5>(),
        // std::make_unique<Day6>(),
        // std::make_unique<Day7>(),
        // std::make_unique<Day8>(),
        // std::make_unique<Day9>(),
        // std::make_unique<Day10>(),
        // std::make_unique<Day11>(),
        // std::make_unique<Day12>(),
        // std::make_unique<Day13>(),
        // std::make_unique<Day14>(),
        // std::make_unique<Day15>(),
        // std::make_unique<Day16>(),
        // std::make_unique<Day17>(),
        // std::make_unique<Day18>(),
        // std::make_unique<Day19>(),
        // std::make_unique<Day20>(),
        // std::make_unique<Day21>(),
        // std::make_unique<Day22>(),
        // std::make_unique<Day23>(),
        // std::make_unique<Day24>(),
        // std::make_unique<Day25>()
    };
    // clang-format on

    if (argc == 1)
    {
        for (const auto& d : days)
        {
            d->run();
        }
    }
    else
    {
        for (int i = 1; i < argc; ++i)
        {
            days[std::atoi(argv[i]) - 1]->run();
        }
    }
}
