add_executable( r-type_client src/main.cpp)

target_link_libraries( r-type_client PRIVATE ECS)
include_directories( ${CMAKE_SOURCE_DIR}/lib/include)
set_target_properties( r-type_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/)
