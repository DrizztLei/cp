cmake_minimum_required(VERSION 3.7)
project(PL0 CXX)

set(BOOST_INCLUDE "/usr/include/boost/")
set(BOOST_LIB "/usr/lib64/")

find_package(Boost COMPONENTS
        filesystem
        system
        thread
        REQUIRED
        )

if(NOT Boost_FOUND)
    message(FATAL_ERROR "error for boost not found")
else()
    message(STATUS "boost searched")
endif()

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp buffer_manager.h buffer_manager.cpp pl0.h set.h set.c pl0.c)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(PL0 ${SOURCE_FILES})

target_link_libraries(PL0 ${Boost_LIBRARIES})

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-lpthread -Wwrite-strings -fpermissive ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "-fpermissive ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "-w ${CMAKE_CXX_FLAGS}")
    message(STATUS "cmake cxx flags ${CMAKE_CXX_FLAGS}")
    message(STATUS "compile for gnu")
endif()

if (TRUE)
    add_compile_options(-std=c++11)
    message(STATUS "add compile option -std=c++11")
    add_compile_options(-fpermissive)
    message(STATUS "add compile option -fpermissive")
    add_compile_options(-lboost_system)
    message(STATUS "add compile option -lboost_system")
    add_compile_options(-lboost_filesystem)
    message(STATUS "add compile option -lpthread")
    add_compile_options(-lpthread)
endif ()

if (unix)
    message(STATUS "configure in *nix")
else ()
    message(STATUS "configure in windows")
endif ()