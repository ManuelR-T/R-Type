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
#include <string>
#include <utility>
#include "../../utils/Logger.hpp"
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
            eng::logInfo("Client disconnected.");
            _closeHandler(_id);
        } else {
            eng::logWarning(ec.message());
        }
        return;
    }
    if (bytes) {
        try {
            handler(_sock, _buff.data(), bytes);
        } catch (std::exception &err) {
            eng::logError(err.what());
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

    _sock.async_receive(
        asio::buffer(_buff, _buff.size()),
        [that = this->shared_from_this(), &handler](asio::error_code ec, std::size_t bytes) {
            that->_handleRead(ec, bytes, handler);
        }
    );
}

/*
    TCPServer
*/

ntw::TCPServer::TCPServer(int port) : _acc(_io, tcp::endpoint(tcp::v4(), port)) {}

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
    eng::logInfo("Send " + std::to_string(size) + " bytes to client.");
    sock.async_write_some(asio::buffer(data, size), [](asio::error_code ec, std::size_t) {
        if (ec) {
            eng::logWarning("Fail to send data to client: " + ec.message() + ".");
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

void ntw::TCPServer::registerDisconnectionHandler(std::function<void(std::size_t)> func)
{
    _mutex.lock();
    _disconnectionHandler = std::move(func);
    _mutex.unlock();
}

void ntw::TCPServer::_handleAccept(asio::error_code ec, const std::shared_ptr<ntw::Session> &session)
{
    if (ec) {
        eng::logWarning("Fail to accept client: " + ec.message() + ".");
        return;
    }
    _mutex.lock();
    this->_session[session->socket().native_handle()] = session;
    _mutex.unlock();

    session->handleClient(_handler);
    _asioRun();
}

void ntw::TCPServer::_asioRun()
{
    auto session = std::make_shared<Session>(tcp::socket(_io), _mutex, [this](std::size_t id) { removeUser(id); });

    _acc.async_accept(session->socket(), [this, session](asio::error_code ec) {
        eng::logInfo("New client connected.");
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
    _mutex.lock();
    _session.erase(id);
    _mutex.unlock();
    if (_disconnectionHandler) {
        _disconnectionHandler(id);
    }
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
    std::shared_ptr<ntw::Session> tmp = _session.at(sock.native_handle());

    _session.erase(sock.native_handle());
    _session[id] = tmp;
    _session.at(id)->id() = id;
}

void ntw::TCPServer::sendToUser(std::size_t id, const char *data, std::size_t size)
{
    std::lock_guard<std::mutex> lock(_mutex);

    if (_session.contains(id)) {
        auto &session = _session.at(id);
        if (session->socket().is_open()) {
            sockWrite(session->socket(), data, size);
        } else {
            eng::logWarning("Trying to send data to a closed socket: " + std::to_string(id) + ".");
        }
    } else {
        eng::logWarning("No session found for: " + std::to_string(id) + ".");
    }
}
