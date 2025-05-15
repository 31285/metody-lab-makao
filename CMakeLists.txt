cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Makao
    VERSION 1.0.0
    LANGUAGES CXX
)

include_directories( "/include")

add_executable(Makao main.cpp
include/card.cpp
include/game.cpp
include/player.cpp
include/gamestate.cpp
)

set(CMAKE_BUILD_TYPE Debug)