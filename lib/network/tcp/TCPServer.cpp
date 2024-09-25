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

void server::Session::handle_read(
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
            handler(sock_, buff_.data(), bytes);
        } catch (std::exception &err) {
            std::cerr << "No such command: ";
            std::cerr.write(buff_.data(), bytes);
            std::cerr << std::endl;
        }
    }
    handle_client(handler);
}

void server::Session::handle_client(std::function<void(tcp::socket &, char *, std::size_t)> &handler)
{
    std::cout << "Session start !\n";
    sock_.async_receive(
        asio::buffer(buff_, buff_.size()),
        [that = this->shared_from_this(), &handler](asio::error_code ec, std::size_t bytes) {
            that->handle_read(ec, bytes, handler);
        }
    );
}

/*
    TCPServer
*/

server::TCPServer::TCPServer(int port) : acc_(io_, tcp::endpoint(tcp::v4(), port)) {}

server::TCPServer::~TCPServer()
{
    if (thread_.joinable()) {
        io_.stop();
        thread_.join();
    }
}

void server::TCPServer::sock_write(tcp::socket &sock_, const char *data, std::size_t size)
{
    sock_.async_write_some(asio::buffer(data, size), [](asio::error_code ec, std::size_t) {
        if (ec) {
            return;
        }
    });
}

void server::TCPServer::register_command(std::function<void(tcp::socket &, char *, std::size_t)> func)
{
    handler_ = std::move(func);
}

void server::TCPServer::handle_accept(asio::error_code ec, std::shared_ptr<server::Session> session)
{
    if (ec) {
        return;
    }

    this->free_session_.push_back(session);

    session->handle_client(handler_);
    asio_run();
}

void server::TCPServer::asio_run()
{
    auto session = std::make_shared<Session>(tcp::socket(io_));

    acc_.async_accept(session->socket(), [this, session](asio::error_code ec) {
        std::cout << "Accepted\n";
        handle_accept(ec, session);
    });
}

void server::TCPServer::run()
{
    thread_ = std::thread([this]() {
        asio_run();
        io_.run();
    });
}

void server::TCPServer::remove_user(std::size_t id)
{
    session_.erase(id);
}

void server::TCPServer::send_to_all_user(const char *data, std::size_t size)
{
    for (auto &[id, session] : session_) {
        if (session->socket().is_open()) {
            sock_write(session->socket(), data, size);
        }
    }
}

void server::TCPServer::add_user(tcp::socket &sock, std::size_t id)
{
    for (auto it = free_session_.begin(); it != free_session_.end(); ++it) {
        if ((*it)->socket().native_handle() == sock.native_handle()) {
            session_[id] = *it;
            free_session_.erase(it);
            break;
        }
    }
}

void server::TCPServer::send_to_user(std::size_t id, const char *data, std::size_t size)
{
    if (session_.contains(id)) {
        auto &session = session_.at(id);
        if (session->socket().is_open()) {
            sock_write(session->socket(), data, size);
        }
    } else {
        std::cerr << "No session found for ID: " << id << std::endl; // ! put log::error
    }
}
