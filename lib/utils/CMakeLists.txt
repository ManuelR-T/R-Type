add_library(UTILS STATIC
    Logger.cpp
    TrackedException.cpp
    ArgParser.cpp
)

target_include_directories(UTILS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
