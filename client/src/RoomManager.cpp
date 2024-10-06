/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** room_manager
*/

#include "RoomManager.hpp"
#include <cstddef>
#include "RTypeTCPProtol.hpp"

std::size_t rtc::RoomManager::getSelfId() const
{
    return _userId;
}

const std::string &rtc::RoomManager::getCurrentRoom() const
{
    return _currentRoom;
}

std::string &rtc::RoomManager::getCurrentRoom()
{
    return _currentRoom;
}

const std::map<std::size_t, rtc::RoomManager::Player> &rtc::RoomManager::getCurrentRoomPlayer() const
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

void rtc::RoomManager::askToCreateRoom(const std::string &roomName, const std::size_t &stage)
{
    rt::TCPPacket<rt::TCPData::CL_CREATE_ROOM> packet{.cmd = rt::TCPCommand::CL_CREATE_ROOM};

    packet.data.stage = stage;
    roomName.copy(packet.data.room_name, sizeof(packet.data.room_name) - 1);
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::askToDeleteRoom(const std::string &roomName)
{
    rt::TCPPacket<rt::TCPData::CL_DELETE_ROOM> packet{.cmd = rt::TCPCommand::CL_DELETE_ROOM};

    roomName.copy(packet.data.room_name, sizeof(packet.data.room_name) - 1);
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::askToJoinRoom(const std::string &roomName)
{
    rt::TCPPacket<rt::TCPData::CL_JOIN_ROOM> packet{.cmd = rt::TCPCommand::CL_JOIN_ROOM};

    packet.data.user_id = _userId;
    roomName.copy(packet.data.room_name, sizeof(packet.data.room_name) - 1);
    _userName.copy(packet.data.user_name, sizeof(packet.data.user_name) - 1);
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::askToLeaveRoom()
{
    rt::TCPPacket<rt::TCPData::CL_LEAVE_ROOM> packet{.cmd = rt::TCPCommand::CL_LEAVE_ROOM};

    packet.data.user_id = _userId;
    _currentRoom.copy(packet.data.room_name, sizeof(packet.data.room_name) - 1);
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::askToBeReady()
{
    rt::TCPPacket<rt::TCPData::CL_READY> packet{.cmd = rt::TCPCommand::CL_READY};

    packet.data.user_id = _userId;
    _currentRoom.copy(packet.data.room_name, sizeof(packet.data.room_name) - 1);
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::RoomManager::askToBeNotReady()
{
    rt::TCPPacket<rt::TCPData::CL_NOT_READY> packet{.cmd = rt::TCPCommand::CL_NOT_READY};

    packet.data.user_id = _userId;
    _currentRoom.copy(packet.data.room_name, sizeof(packet.data.room_name) - 1);
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}
