add_library(FACTORY STATIC
    systems/collision.cpp
    systems/control_move.cpp
    systems/control_special.cpp
    systems/draw.cpp
    systems/draw_player_beam_bar.cpp
    systems/draw_team_data.cpp
    systems/draw_score.cpp
    systems/draw_player_health_bar.cpp
    systems/sprite_system.cpp
    systems/position.cpp
    systems/missiles_stop.cpp
    systems/client_share_movement.cpp
    systems/server_share_movement.cpp
    systems/health_check.cpp
    systems/health_mob_check.cpp
    systems/parallax.cpp
    EntityFactory.cpp
    ServerEntityFactory.cpp
    ClientEntityFactory.cpp
    systems/ai_act.cpp
    WaveManager.cpp
    SoundManager.cpp
)

target_include_directories(FACTORY PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(FACTORY PUBLIC nlohmann_json ECS NETWORK sfml-system sfml-window sfml-graphics sfml-audio ImGui-SFML::ImGui-SFML)