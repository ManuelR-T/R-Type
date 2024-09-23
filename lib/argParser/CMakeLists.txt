add_library(ARGPARSER STATIC
    argParser.cpp
)

target_include_directories(ARGPARSER PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
