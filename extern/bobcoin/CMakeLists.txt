cmake_minimum_required(VERSION 3.0)
project(bobcoin)

include_directories(include)

add_library(bobcoin STATIC src/bobcoin.cpp)
target_include_directories(bobcoin PUBLIC include)
