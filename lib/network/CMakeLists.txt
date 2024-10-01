add_library(NETWORK STATIC
    tcp/TCPServer.cpp
    tcp/TCPClient.cpp
    udp/UDPServer.cpp
    udp/UDPClient.cpp
)

target_include_directories(NETWORK PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(NETWORK PUBLIC asio)

if (WIN32)
    target_link_libraries(NETWORK PUBLIC ws2_32 mswsock bcrypt)
endif()
