# Create library
project (optimizers)

set (
  optimizers_files
  optimizers
  normalform
)

add_library(optimizers ${optimizers_files})
