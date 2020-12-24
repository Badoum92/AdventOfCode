cmake_minimum_required(VERSION 3.1)

project(advent_of_code)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -O3")

add_executable(aoc
    src/main.cc
    src/common.cc
    src/days/day1.cc
    src/days/day2.cc
    src/days/day3.cc
    src/days/day4.cc
    src/days/day5.cc
    src/days/day6.cc
    src/days/day7.cc
    src/days/day8.cc
    src/days/day9.cc
    src/days/day10.cc
    src/days/day11.cc
    src/days/day12.cc
    src/days/day13.cc
    src/days/day14.cc
    src/days/day15.cc
    src/days/day16.cc
    src/days/day17.cc
    src/days/day18.cc
    src/days/day19.cc
    src/days/day20.cc
    src/days/day21.cc
    src/days/day22.cc
    src/days/day23.cc
    src/days/day24.cc
)

target_include_directories(aoc
    PRIVATE src
)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
