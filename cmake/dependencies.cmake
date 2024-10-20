include(cmake/CPM.cmake)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(SFML_USE_STATIC_STD_LIBS ON CACHE BOOL "" FORCE)

CPMAddPackage("gh:SFML/SFML#2.6.x")

CPMAddPackage(
    NAME imgui
    GITHUB_REPOSITORY ocornut/imgui
    GIT_TAG v1.89.2
)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
message(STATUS "IMGUI_DIR: ${IMGUI_DIR}")

CPMAddPackage(
    NAME ImGui-SFML
    GITHUB_REPOSITORY SFML/imgui-sfml
    GIT_TAG 2.6.x
)

set(ASIO_TAG "asio-1-31-0")
set(ASIO_NO_DEPRECATED ON)
CPMAddPackage(
  NAME asiocmake
  GIT_REPOSITORY "https://github.com/OlivierLDff/asio.cmake"
  GIT_TAG "main"
)

CPMAddPackage(
  NAME nlohmann_json
  GITHUB_REPOSITORY nlohmann/json
  GIT_TAG v3.11.3
)

CPMAddPackage(
  NAME sfml_clandle
  GITHUB_REPOSITORY MiguelMJ/Candle
  GIT_TAG v1.0.2
)
target_compile_definitions(Candle-s PUBLIC RADIAL_LIGHT_FIX)

# CPMAddPackage(
#     NAME sol2
#     GITHUB_REPOSITORY ThePhD/sol2
#     GIT_TAG v3.3.0
# )

# CPMAddPackage(
#     NAME lua_static
#     GITHUB_REPOSITORY walterschell/Lua
#     GIT_TAG v5.4.5
# )
