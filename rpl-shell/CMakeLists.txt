cmake_minimum_required (VERSION 2.8.11)

project (rpl-shell)

option(EXPANSION "Expansion of trees enabled" OFF)

if(EXPANSION)
	add_definitions(-DEXPANSION)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -Wall -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -O3 -Wall")

add_subdirectory (rpl)
add_executable (rplsh rplsh.cpp)
target_link_libraries (rplsh LINK_PUBLIC rpl)
target_link_libraries(rplsh LINK_PUBLIC readline)
