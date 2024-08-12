cmake_minimum_required(VERSION 3.10)

project(Trafficpp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG yaml-cpp-0.7.0
)
FetchContent_MakeAvailable(yaml-cpp)

find_library(PCAP_LIBRARY pcap)
find_path(PCAP_INCLUDE_DIR pcap.h)

if(NOT PCAP_LIBRARY OR NOT PCAP_INCLUDE_DIR)
    message(FATAL_ERROR "libpcap not found")
endif()

add_executable(Trafficpp src/argparse.cpp src/color.cpp src/traffic.cpp src/main.cpp)

target_include_directories(Trafficpp PRIVATE include ${PCAP_INCLUDE_DIR})
target_link_libraries(Trafficpp PUBLIC yaml-cpp::yaml-cpp ${PCAP_LIBRARY})

set_target_properties(Trafficpp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
