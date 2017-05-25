# Create library
project (rewriting)

set (
  rr_files
  rewriter
  rewrules
)

add_library(rewriting ${rr_files})
