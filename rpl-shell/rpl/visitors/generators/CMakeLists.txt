# Create library
project (generators)

set (
  generator_files
  ffcode
)

add_library(generators ${generator_files})
