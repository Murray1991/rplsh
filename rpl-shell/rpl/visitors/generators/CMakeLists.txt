# Create library
project (generators)

set (
  generator_files
  ffcode
  ffmapper
  compseq_setter
)

add_library(generators ${generator_files})
