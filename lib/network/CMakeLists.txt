add_library(NETWORK STATIC
    tcp/TCPServer.cpp
    tcp/TCPClient.cpp
    udp/UDPServer.cpp
    udp/UDPClient.cpp
)

target_include_directories(NETWORK PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
