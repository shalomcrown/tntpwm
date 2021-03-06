cmake_minimum_required(VERSION 2.6)
project(tntpwm)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/Modules/")

find_package(TNTNET REQUIRED)
find_package(WIRINGPI REQUIRED)
find_package (Threads)

add_subdirectory(src)
add_subdirectory(Modules)
add_compile_options(-std=c++11 -pthread)


file(GLOB HEADERS
    src/*.h*
)

set(ECPP_FILES src/mainpage.ecpp src/changeValue.ecpp src/status.ecpp )
SET(SOURCE_FILES src/main.cpp src/baro-bmp180.cpp src/accel.cpp )

configure_file(log.xml ${CMAKE_CURRENT_SOURCE_DIR}/log.xml COPYONLY)
configure_file(src/baro-bmp180.hpp ${CMAKE_CURRENT_BINARY_DIR}/src/baro-bmp180.hpp COPYONLY)
configure_file(src/accel.hpp ${CMAKE_CURRENT_BINARY_DIR}/src/accel.hpp COPYONLY)


foreach(comp ${ECPP_FILES})
    get_filename_component(comp_name ${comp} NAME_WE)

    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${comp}.cpp
        COMMAND ecppc -I src -o ${CMAKE_CURRENT_BINARY_DIR}/${comp}.cpp -v -n ${comp_name} ${CMAKE_CURRENT_SOURCE_DIR}/${comp}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${comp} )

    SET(SOURCE_FILES ${SOURCE_FILES} ${CMAKE_CURRENT_BINARY_DIR}/${comp}.cpp )

endforeach()


add_executable(tntpwm ${SOURCE_FILES} )

target_include_directories(tntpwm PRIVATE ${TNTNET_INCLUDE_DIRS} src )
target_link_libraries(tntpwm ${TNTNET_LIBRARIES} ${WIRINGPI_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})


install(TARGETS tntpwm RUNTIME DESTINATION bin)

