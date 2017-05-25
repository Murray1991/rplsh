# Create library
project (nodes)

set (
  nodes_files
  skeletons
  verbs
)

add_library(nodes ${nodes_files})
