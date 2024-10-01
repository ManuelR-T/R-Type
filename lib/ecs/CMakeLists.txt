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
)

target_include_directories(ECS PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils/
    ${CMAKE_CURRENT_SOURCE_DIR}/../network/
    ${CMAKE_CURRENT_SOURCE_DIR}/../network/udp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

CPMAddPackage(
    NAME SFML
    GITHUB_REPOSITORY SFML/SFML
    GIT_TAG master
    FIND_PACKAGE_ARGUMENTS "COMPONENTS system window graphics REQUIRED"
)

if(WIN32)
    target_link_libraries(ECS PRIVATE mingw_stdthreads)
    target_include_directories(ECS PRIVATE mingw_stdthreads)
endif()


target_link_libraries(ECS PUBLIC sfml-system sfml-window sfml-graphics)
