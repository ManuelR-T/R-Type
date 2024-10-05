add_library(ECS STATIC
    systems/collision.cpp
    systems/control_move.cpp
    systems/control_special.cpp
    systems/draw.cpp
    systems/sprite_system.cpp
    systems/position.cpp
    systems/missiles_stop.cpp
    systems/share_movement.cpp
    systems/health_check.cpp
    systems/ai_act.cpp
    core/shared_entity.cpp
    factory/EntityFactory.cpp
    factory/ServerEntityFactory.cpp
    factory/ClientEntityFactory.cpp
)

target_include_directories(ECS PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(ECS PUBLIC nlohmann_json NETWORK sfml-system sfml-window sfml-graphics)
