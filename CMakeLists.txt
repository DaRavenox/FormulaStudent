cmake_minimum_required(VERSION 3.9)
project(FormulaStudent)

set(CMAKE_CXX_STANDARD 11)

add_executable(FormulaStudent watchdog.cpp watchdog.h Listener.c)