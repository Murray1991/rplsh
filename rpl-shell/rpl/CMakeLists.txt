# Create library
project (rpl)

include_directories(.)

# Add subdirectories
add_subdirectory (nodes)
add_subdirectory (parser)
add_subdirectory (visitors)
add_subdirectory (rewriting)
add_subdirectory (environment)
add_subdirectory (error_report)

# Add source file
add_library(rpl interpreter.cpp)

# Target link libraries
target_link_libraries(rpl parser visitors rewriting)

# Make sure the compiler can find include files
target_include_directories (rpl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
