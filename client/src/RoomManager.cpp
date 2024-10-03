/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** room_manager
*/

#include "RoomManager.hpp"
#include "RTypeTCPProtol.hpp"

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
    rt::TCPPacket<rt::TCPData::CL_CREATE_ROOM> packet{.cmd = rt::TCPCommand::CL_CREATE_ROOM};

    std::memcpy(packet.data.room_name, roomName.c_str(), roomName.size());
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::askToDeleteRoom(const std::string &roomName)
{
    rt::TCPPacket<rt::TCPData::CL_DELETE_ROOM> packet{.cmd = rt::TCPCommand::CL_DELETE_ROOM};

    std::memcpy(packet.data.room_name, roomName.c_str(), roomName.size());
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::askToJoinRoom(const std::string &roomName)
{
    rt::TCPPacket<rt::TCPData::CL_JOIN_ROOM> packet{.cmd = rt::TCPCommand::CL_JOIN_ROOM};

    packet.data.user_id = _userId;
    std::memcpy(packet.data.room_name, roomName.c_str(), roomName.size());
    std::memcpy(packet.data.user_name, _userName.c_str(), _userName.size());
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::askToLeaveRoom()
{
    rt::TCPPacket<rt::TCPData::CL_LEAVE_ROOM> packet{.cmd = rt::TCPCommand::CL_LEAVE_ROOM};

    packet.data.user_id = _userId;
    std::memcpy(packet.data.room_name, _currentRoom.c_str(), _currentRoom.size());
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::askToBeReady()
{
    rt::TCPPacket<rt::TCPData::CL_READY> packet{.cmd = rt::TCPCommand::CL_READY};

    packet.data.user_id = _userId;
    std::memcpy(packet.data.room_name, _currentRoom.c_str(), _currentRoom.size());
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::askToBeNotReady()
{
    rt::TCPPacket<rt::TCPData::CL_NOT_READY> packet{.cmd = rt::TCPCommand::CL_NOT_READY};

    packet.data.user_id = _userId;
    std::memcpy(packet.data.room_name, _currentRoom.c_str(), _currentRoom.size());
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}
