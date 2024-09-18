add_library(ECS STATIC
    systems/colision.cpp
    systems/control.cpp
    systems/draw.cpp
    systems/position.cpp
    systems/missiles_stop.cpp
)

target_include_directories(ECS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
target_link_libraries(ECS PUBLIC sfml-system sfml-window sfml-graphics)
