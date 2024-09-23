add_executable( r-type_client src/main.cpp)
target_link_libraries( r-type_client PRIVATE ECS PRIVATE NETWORK PRIVATE INCLUDE PRIVATE ARGPARSER)
include_directories( ${CMAKE_SOURCE_DIR}/lib/include)
set_target_properties( r-type_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)
