cmake_minimum_required(VERSION 3.10)
project(signaltest)

set(CMAKE_CXX_STANDARD 11)

add_executable(signaler
        signal.cpp)

add_executable(sigaction
        asignal.h
        asignal.cpp
        sigaction.cpp)
