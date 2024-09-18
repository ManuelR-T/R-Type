add_executable( r-type_server src/main.cpp)

target_link_libraries( r-type_server PRIVATE ECS PRIVATE NETWORK)
include_directories( ${CMAKE_SOURCE_DIR}/lib/include, ${CMAKE_SOURCE_DIR}/lib/network/udp)
set_target_properties( r-type_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
