add_library(UTILS STATIC
    log.cpp
    TrackedException.cpp
)

target_include_directories(UTILS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
