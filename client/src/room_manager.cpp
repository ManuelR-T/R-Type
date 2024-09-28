/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** room_manager
*/

#include "GameProtocol.hpp"
#include "RoomManager.hpp"

const std::string &rtc::RoomManager::getCurrentRoom() const
{
    return _currentRoom;
}

std::string &rtc::RoomManager::getCurrentRoom()
{
    return _currentRoom;
}

const std::map<std::string, bool> &rtc::RoomManager::getCurrentRoomPlayer() const
{
    return _rooms.at(_currentRoom).player;
}

const std::map<std::string, rtc::RoomManager::RoomContent> &rtc::RoomManager::getRooms() const
{
    return _rooms;
}

std::map<std::string, rtc::RoomManager::RoomContent> &rtc::RoomManager::getRooms()
{
    return _rooms;
}

void rtc::RoomManager::askToCreateRoom(const std::string &roomName)
{
    rt::TcpPacket packet{.cmd = rt::TcpCommand::CL_CREATE_ROOM};

    std::memcpy(packet.body.cl_create_room.room_name, roomName.c_str(), roomName.size());
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::askToDeleteRoom(const std::string &roomName)
{
    rt::TcpPacket packet{.cmd = rt::TcpCommand::CL_DELETE_ROOM};

    std::memcpy(packet.body.cl_delete_room.room_name, roomName.c_str(), roomName.size());
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::askToJoinRoom(const std::string &roomName)
{
    rt::TcpPacket packet{.cmd = rt::TcpCommand::CL_JOIN_ROOM};

    packet.body.cl_join_room.user_id = _userId;
    std::memcpy(packet.body.cl_join_room.room_name, roomName.c_str(), roomName.size());
    std::memcpy(packet.body.cl_join_room.user_name, _userName.c_str(), _userName.size());
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::askToLeaveRoom()
{
    rt::TcpPacket packet{.cmd = rt::TcpCommand::CL_LEAVE_ROOM};

    packet.body.cl_leave_room.user_id = _userId;
    std::memcpy(packet.body.cl_leave_room.room_name, _currentRoom.c_str(), _currentRoom.size());
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::askToBeReady()
{
    rt::TcpPacket packet{.cmd = rt::TcpCommand::CL_READY};

    packet.body.cl_ready.user_id = _userId;
    std::memcpy(packet.body.cl_ready.room_name, _currentRoom.c_str(), _currentRoom.size());
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::askToBeNotReady()
{
    rt::TcpPacket packet{.cmd = rt::TcpCommand::CL_NOT_READY};

    packet.body.cl_not_ready.user_id = _userId;
    std::memcpy(packet.body.cl_not_ready.room_name, _currentRoom.c_str(), _currentRoom.size());
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}
