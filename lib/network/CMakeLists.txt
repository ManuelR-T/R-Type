add_library(NETWORK STATIC
    tcp/TCPServer.cpp
    udp/UDPServer.cpp
)

target_include_directories(NETWORK PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
