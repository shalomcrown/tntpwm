cmake_minimum_required(VERSION 2.6)
project(tntpwm)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/Modules/")

find_package(TNTNET REQUIRED)

add_subdirectory(src)
add_subdirectory(Modules)

add_definitions(-std=c++11)


add_executable(tntpwm src/main.cpp src/mainpage.ecpp)

target_include_directories(tntpwm ${TNTNET_INCLUDE_DIRS})
target_link_libraries(tntpwm ${TNTNET_LIBRARIES})


install(TARGETS tntpwm RUNTIME DESTINATION bin)

