cmake_minimum_required(VERSION 3.11)
project(cmqtt C)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")

find_library(mosquitto libmosquitto.so)
message(STATUS ${mosquitto})

add_executable(cmqtt src/main.c)
target_link_libraries(cmqtt PRIVATE ${mosquitto})
