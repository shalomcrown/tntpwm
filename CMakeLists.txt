cmake_minimum_required(VERSION 2.6)
project(tntpwm)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/Modules/")

find_package(TNTNET REQUIRED)

add_subdirectory(src)
add_subdirectory(Modules)
add_definitions(-std=c++11)


set(ECPP_FILES src/mainpage.ecpp)
SET(SOURCE_FILES src/main.cpp)

foreach(comp ${ECPP_FILES})
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${comp}.cpp
        COMMAND ecppc -o ${CMAKE_CURRENT_BINARY_DIR}/${comp}.cpp -v -n ${comp} ${CMAKE_CURRENT_SOURCE_DIR}/${comp}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${comp})

    SET(SOURCE_FILES ${SOURCE_FILES} ${CMAKE_CURRENT_BINARY_DIR}/${comp}.cpp )

endforeach()


add_executable(tntpwm ${SOURCE_FILES} )

target_include_directories(tntpwm PRIVATE ${TNTNET_INCLUDE_DIRS})
target_link_libraries(tntpwm ${TNTNET_LIBRARIES})


install(TARGETS tntpwm RUNTIME DESTINATION bin)

