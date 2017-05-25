# Create library
project (visitors)

add_subdirectory (annotators)
add_subdirectory (optimizers)

set (
  visitors_files
  visitors
)

add_library(visitors ${visitors_files})

target_link_libraries(visitors optimizers annotators environment)
