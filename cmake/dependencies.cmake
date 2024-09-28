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
