#include "common.hh"

int main(int argc, char** argv)
{
    Day* days[] = {new Day1(), new Day2(), new Day3(), new Day4(), new Day5(),
                   new Day6(), new Day7(), new Day8(), new Day9(), new Day10()};

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

    for (const auto& d : days)
    {
        delete d;
    }
}
