cmake_minimum_required(VERSION 2.8)
project(lcm_benchmark)

add_definitions(-std=c++11)

find_package(lcm REQUIRED)
include(${LCM_USE_FILE})

lcm_wrap_types(
    CPP_HEADERS cpp_install_headers CPP11
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/lcm_gen
    types/ByteVectorStamped.lcm
    types/Duration.lcm)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/lcm_gen)

lcm_add_library(bytevector-cpp CPP ${cpp_install_headers})

lcm_install_headers(
    ${cpp_install_headers}
    DESTINATION include/lcmtypes
)

add_executable(ping_node ping_node.cpp)
lcm_target_link_libraries(ping_node lcm)
lcm_target_link_libraries(ping_node bytevector-cpp pthread)

add_executable(pong_node pong_node.cpp)
lcm_target_link_libraries(pong_node lcm)
lcm_target_link_libraries(pong_node bytevector-cpp)
