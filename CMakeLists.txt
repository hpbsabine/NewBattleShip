cmake_minimum_required(VERSION 3.10)
project(NewBattleShip)

# Add executable
add_executable(NewBattleShip main.cpp Board.cpp)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
