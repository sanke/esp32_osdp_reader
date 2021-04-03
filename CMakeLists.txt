cmake_minimum_required(VERSION 3.17)
project(osdp_lib)

set(CMAKE_CXX_STANDARD 14)

add_library(osdp_lib osdp_lib.cpp osdp_lib.h osdp_packet.cpp osdp_packet.h osdp_utils.cpp osdp_utils.h)

target_include_directories(osdp_lib PRIVATE "C:\\Program Files (x86)\\Arduino\\hardware\\arduino\\avr\\cores\\arduino")