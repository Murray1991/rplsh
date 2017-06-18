# Create library
project (visitors)

add_subdirectory (annotators)
add_subdirectory (optimizers)
add_subdirectory (generators)
add_subdirectory (evaluators)

set (
  visitors_files
  visitors
)

add_library(visitors ${visitors_files})
target_link_libraries(visitors evaluators optimizers generators annotators environment)
