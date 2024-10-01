add_library(NETWORK STATIC
    tcp/TCPServer.cpp
    tcp/TCPClient.cpp
    udp/UDPServer.cpp
    udp/UDPClient.cpp
)

target_link_libraries(NETWORK PRIVATE asio
)

target_include_directories(NETWORK PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../utils
)

if(WIN32)
    target_link_libraries(NETWORK PUBLIC mingw_stdthreads)
    target_include_directories(NETWORK PUBLIC mingw_stdthreads)
endif()
