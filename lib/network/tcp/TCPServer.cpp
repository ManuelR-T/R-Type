/*
** EPITECH PROJECT, 2024
** network
** File description:
** TCPServer
*/

#include "TCPServer.hpp"

#include <asio/placeholders.hpp>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>
#include <asio/completion_condition.hpp>
#include <asio/error_code.hpp>

/*
    Session
*/

void ntw::Session::_handleRead(
    asio::error_code ec,
    std::size_t bytes,
    std::function<void(tcp::socket &, char *, std::size_t)> &handler
)
{
    if (ec) {
        if (ec == asio::error::eof || ec == asio::error::connection_reset) {
            std::cerr << "Client disconnected: " << ec.message() << std::endl; // ! log
            return;
        } else {
            std::cerr << "Error on receive: " << ec.message() << std::endl; // ! log::error
            return;
        }
        return;
    }
    if (bytes) {
        try {
            handler(_sock, _buff.data(), bytes);
        } catch (std::exception &err) {
            std::cerr << "No such command: ";
            std::cerr.write(_buff.data(), bytes);
            std::cerr << std::endl;
        }
    }
    handleClient(handler);
}

void ntw::Session::handleClient(std::function<void(tcp::socket &, char *, std::size_t)> &handler)
{
    std::lock_guard<std::mutex> lock(_serverMutex);

    std::cout << "Session start !\n";
    _sock.async_receive(
        asio::buffer(_buff, _dataSize),
        [that = this->shared_from_this(), &handler](asio::error_code ec, std::size_t bytes) {
            that->_handleRead(ec, bytes, handler);
        }
    );
}

/*
    TCPServer
*/

ntw::TCPServer::TCPServer(int port, std::size_t dataSize)
    : _acc(_io, tcp::endpoint(tcp::v4(), port)), _dataSize(dataSize)
{
}

ntw::TCPServer::~TCPServer()
{
    _io.stop();
    if (_thread.joinable()) {
        _thread.join();
    }
    _mutex.lock();
    for (auto &[id, session] : _session) {
        session->socket().close();
    }
    _mutex.unlock();
}

void ntw::TCPServer::sockWrite(tcp::socket &sock, const char *data, std::size_t size)
{
    std::cout << "SENT TO CLIENT SIZE:" << size << "\n";
    sock.async_write_some(asio::buffer(data, size), [](asio::error_code ec, std::size_t) {
        if (ec) {
            std::cout << "FAIL TO SEND DATA TO USER !";
            return;
        }
    });
}

void ntw::TCPServer::registerCommand(std::function<void(tcp::socket &, char *, std::size_t)> func)
{
    _mutex.lock();
    _handler = std::move(func);
    _mutex.unlock();
}

void ntw::TCPServer::_handleAccept(asio::error_code ec, const std::shared_ptr<ntw::Session> &session)
{
    if (ec) {
        return;
    }

    _mutex.lock();
    this->_freeSession.push_back(session);
    _mutex.unlock();

    session->handleClient(_handler);
    _asioRun();
}

void ntw::TCPServer::_asioRun()
{
    auto session = std::make_shared<Session>(tcp::socket(_io), _mutex, _dataSize);

    _acc.async_accept(session->socket(), [this, session](asio::error_code ec) {
        std::cout << "Accepted\n";
        _handleAccept(ec, session);
    });
}

void ntw::TCPServer::run()
{
    _thread = std::thread([this]() {
        _asioRun();
        _io.run();
    });
}

void ntw::TCPServer::removeUser(std::size_t id)
{
    std::lock_guard<std::mutex> lock(_mutex);

    _session.erase(id);
}

void ntw::TCPServer::sendToAllUser(const char *data, std::size_t size)
{
    std::lock_guard<std::mutex> lock(_mutex);

    for (auto &[id, session] : _session) {
        if (session->socket().is_open()) {
            sockWrite(session->socket(), data, size);
        }
    }
}

void ntw::TCPServer::addUser(tcp::socket &sock, std::size_t id)
{
    std::lock_guard<std::mutex> lock(_mutex);

    for (auto it = _freeSession.begin(); it != _freeSession.end(); ++it) {
        if ((*it)->socket().native_handle() == sock.native_handle()) {
            _session[id] = *it;
            _freeSession.erase(it);
            break;
        }
    }
}

void ntw::TCPServer::sendToUser(std::size_t id, const char *data, std::size_t size)
{
    std::lock_guard<std::mutex> lock(_mutex);

    if (_session.contains(id)) {
        auto &session = _session.at(id);
        if (session->socket().is_open()) {
            sockWrite(session->socket(), data, size);
        } else {
            std::cerr << "SOCKET NOT OPEN !!!!!!!" << id << std::endl; // ! put log::error
        }
    } else {
        std::cerr << "No session found for ID: " << id << std::endl; // ! put log::error
    }
}
