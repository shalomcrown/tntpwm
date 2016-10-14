cmake_minimum_required(VERSION 2.6)
project(tntpwm)

add_executable(tntpwm main.cpp)

install(TARGETS tntpwm RUNTIME DESTINATION bin)
