/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** setup_tcp_connection
*/

#include "GameManager.hpp"
#include "GameProtocol.hpp"

void rtc::GameManager::_setupTcpConnection()
{
    this->_registerTcpResponse();
    _tcpClient.registerHandler([this](const char *data, std::size_t size) {
        _tcpResponseHandler.handleResponse(data, size);
    });

    _tcpClient.run();

    {
        rt::TcpPacket packet{.cmd = rt::TcpCommand::CL_NEW_USER};
        packet.body.cl_new_user.user_id = _userId;
        _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
    }
    {
        rt::TcpPacket packet{.cmd = rt::TcpCommand::CL_ROOM_LIST};
        packet.body.cl_room_list.user_id = _userId;
        _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
    }
}
