cmake_minimum_required(VERSION 3.22)
project(Protocol5)

set(CMAKE_CXX_STANDARD 14)

add_executable(Protocol5 main.cpp lib/PhysicalLayer.cpp lib/PhysicalLayer.h lib/Frame.cpp lib/Frame.h lib/Protocol.cpp lib/Protocol.h lib/timer.cpp lib/timer.h)
