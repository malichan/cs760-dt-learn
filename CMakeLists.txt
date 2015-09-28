cmake_minimum_required(VERSION 2.8)
project(dt-learn)

set(CMAKE_CXX_FLAGS "-std=c++11")

add_executable(dt-learn dt-learn.cpp Feature.cpp Instance.cpp Dataset.cpp Split.cpp DecisionTree.cpp)