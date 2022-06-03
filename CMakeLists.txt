cmake_minimum_required(VERSION 3.22)
project(biological_neurons)

set(CMAKE_CXX_STANDARD 17)

add_executable(biological_neurons main.cpp src/neuron.cpp src/neuron.h src/config.h)
