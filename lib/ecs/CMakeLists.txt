add_library(ECS STATIC
    systems/collision.cpp
    systems/control.cpp
    systems/draw.cpp
    systems/position.cpp
    systems/share_movement.cpp
    core/shared_entity.cpp
)

target_include_directories(ECS PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/
    ${CMAKE_CURRENT_SOURCE_DIR}/../network/
    ${CMAKE_CURRENT_SOURCE_DIR}/../network/udp)

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
target_link_libraries(ECS PUBLIC sfml-system sfml-window sfml-graphics)
