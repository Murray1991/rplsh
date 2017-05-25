# Create library called "error"
project (parser)

set (
  parser_files
  lexer
  parser
  token
)

add_library(parser ${parser_files})

target_link_libraries(parser error_report nodes)
