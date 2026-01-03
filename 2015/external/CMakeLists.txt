# External dependencies

# Add zdk header-only library
add_library(ZDK INTERFACE)
target_include_directories(ZDK INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
