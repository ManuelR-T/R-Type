include(cmake/CPM.cmake)

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

if (WIN32)
  CPMAddPackage(
    NAME mingw_stdthreads
    GIT_REPOSITORY "https://github.com/meganz/mingw-std-threads"
    GIT_TAG "master"
  )
endif()
