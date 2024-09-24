add_executable( r-type_client
    src/main.cpp
    src/lobby/event.cpp
    src/lobby/run.cpp
    src/lobby/initialization/init.cpp
    src/lobby/initialization/initText.cpp
    src/lobby/initialization/createMovableBox.cpp
    src/lobby/initialization/createStaticBox.cpp
    src/lobby/playersList.cpp
)

target_link_libraries( r-type_client PRIVATE ECS PRIVATE NETWORK PRIVATE INCLUDE)
include_directories( ${CMAKE_SOURCE_DIR}/lib/include, ${CMAKE_SOURCE_DIR}/lib/network/udp, include)
set_target_properties( r-type_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)
