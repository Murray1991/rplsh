# Create library called "error"
project (error)

set (
  error_files
  error_report.hpp
  error_report.cpp
  style.hpp
)

# Add library target error
add_library(error ${error_files})
