/*
** EPITECH PROJECT, 2024
** network
** File description:
** TCPServer
*/

#pragma once

#include <asio.hpp>
#include <asio/ip/tcp.hpp>
#include <cstddef>
#include <functional>
#include "../AsioServer.hpp"
#include "../BuffSize.hpp"

#include <mutex>
#include <unordered_map>

using asio::ip::tcp;

namespace ntw {

/**
 * @class Session
 * @brief Manages individual client connections.
 *
 * The `Session` class encapsulates a single client connection, handling communication
 * between the server and the connected client. It reads incoming data and processes
 * it using the provided handler functions.
 */
class Session : public std::enable_shared_from_this<Session> {
    public:
    /**
     * @brief Constructs a new Session object.
     *
     * Initializes the session with a TCP socket, a reference to the server mutex,
     * and a handler function to be called upon disconnection.
     *
     * @param sock The TCP socket representing the client connection.
     * @param serverMutex A mutex to ensure thread-safe operations on the server.
     * @param closeHandler A function to handle session closure, taking the session ID as a parameter.
     */
    Session(tcp::socket &&sock, std::mutex &serverMutex, const std::function<void(std::size_t)> &closeHandler)
        : _sock(std::move(sock)), _serverMutex(serverMutex), _closeHandler(closeHandler), _id(_sock.native_handle())
    {
    }

    /**
     * @brief Destroys the Session object.
     *
     * Default destructor.
     */
    virtual ~Session() = default;

    /**
     * @brief Starts handling client communication.
     *
     * Initiates asynchronous read operations to handle incoming data from the client.
     *
     * @param handler A function to process the received data, taking the socket, buffer, and size as parameters.
     */
    void handleClient(std::function<void(tcp::socket &, char *, std::size_t)> &handler);

    /**
     * @brief Retrieves the underlying TCP socket.
     *
     * @return A reference to the TCP socket associated with this session.
     */
    tcp::socket &socket()
    {
        return _sock;
    }

    /**
     * @brief Retrieves the session ID.
     *
     * @return A reference to the session's unique identifier.
     */
    std::size_t &id()
    {
        return _id;
    }

    private:
    /**
     * @brief Handles read operations from the client.
     *
     * Processes the data read from the client socket and invokes the provided handler.
     *
     * @param ec The error code resulting from the read operation.
     * @param bytes The number of bytes read.
     * @param handler A function to process the received data.
     */
    void _handleRead(
        asio::error_code ec,
        std::size_t bytes,
        std::function<void(tcp::socket &, char *, std::size_t)> &handler
    );

    tcp::socket _sock;                                    ///< The TCP socket for client communication.
    std::mutex &_serverMutex;                             ///< Reference to the server's mutex for thread safety.
    std::array<char, BUFF_SIZE> _buff;                    ///< Buffer to store incoming data.
    const std::function<void(std::size_t)> _closeHandler; ///< Handler function to call upon session closure.
    std::size_t _id;                                      ///< Unique identifier for the session.
};

/**
 * @class TCPServer
 * @brief Implements a TCP server using Asio for asynchronous networking.
 *
 * The `TCPServer` class manages client connections, handles incoming data, and facilitates
 * communication between connected clients. It allows registering custom command handlers
 * and disconnection handlers to extend its functionality.
 */
class TCPServer : public ntw::AsioServer {
    public:
    /**
     * @brief Constructs a new TCPServer object.
     *
     * Initializes the server to listen on the specified port.
     *
     * @param port The port number on which the server will listen for incoming connections.
     */
    TCPServer(int port);

    /**
     * @brief Destroys the TCPServer object.
     *
     * Cleans up resources and stops the server.
     */
    ~TCPServer() override;

    /**
     * @brief Starts the server's event loop.
     *
     * Runs the Asio I/O context to begin accepting and handling client connections.
     */
    void run() override;

    /**
     * @brief Registers a command handler.
     *
     * Associates a function to handle incoming data from clients. The handler should
     * define how to process the received data.
     *
     * @param func A function that processes incoming data, taking the client's socket,
     *             a buffer containing the data, and the size of the data as parameters.
     */
    void registerCommand(std::function<void(tcp::socket &, char *, std::size_t)> func);

    /**
     * @brief Registers a disconnection handler.
     *
     * Associates a function to handle client disconnections. The handler receives the
     * unique identifier of the disconnected session.
     *
     * @param func A function that handles disconnections, taking the session ID as a parameter.
     */
    void registerDisconnectionHandler(std::function<void(std::size_t)> func);

    /**
     * @brief Sends data to a specific client socket.
     *
     * Writes the provided data to the specified client's socket.
     *
     * @param sock The TCP socket of the target client.
     * @param data Pointer to the data to be sent.
     * @param size The size of the data in bytes.
     */
    void sockWrite(tcp::socket &sock, const char *data, std::size_t size);

    /**
     * @brief Removes a user session from the server.
     *
     * Deletes the session associated with the given session ID, effectively disconnecting the user.
     *
     * @param id The unique identifier of the session to be removed.
     */
    void removeUser(std::size_t id);

    /**
     * @brief Adds a new user session to the server.
     *
     * Stores the session associated with the given socket and assigns it a unique identifier.
     *
     * @param sock The TCP socket of the new client.
     * @param id The unique identifier to assign to the new session.
     */
    void addUser(tcp::socket &sock, size_t id);

    /**
     * @brief Sends data to a specific user.
     *
     * Sends the provided data to the client associated with the given session ID.
     *
     * @param id The unique identifier of the target session.
     * @param data Pointer to the data to be sent.
     * @param size The size of the data in bytes.
     */
    void sendToUser(size_t id, const char *data, std::size_t size);

    /**
     * @brief Sends data to all connected users.
     *
     * Broadcasts the provided data to every connected client.
     *
     * @param data Pointer to the data to be sent.
     * @param size The size of the data in bytes.
     */
    void sendToAllUser(const char *data, std::size_t size);

    private:
    /**
     * @brief Runs the Asio I/O context in a separate thread.
     *
     * This method is executed in a dedicated thread to handle asynchronous operations.
     */
    void _asioRun() override;

    /**
     * @brief Handles the acceptance of new client connections.
     *
     * Called when a new client attempts to connect. Creates a new session for the client
     * and starts handling its communication.
     *
     * @param ec The error code resulting from the accept operation.
     * @param session A shared pointer to the newly created session.
     */
    void _handleAccept(asio::error_code ec, const std::shared_ptr<Session> &session);

    std::thread _thread;  ///< Thread in which the Asio I/O context runs.
    asio::io_context _io; ///< Asio I/O context for managing asynchronous operations.
    tcp::acceptor _acc;   ///< Acceptor object to listen for incoming TCP connections.
    std::function<void(tcp::socket &, char *, std::size_t)> _handler; ///< Function to handle incoming data.
    std::function<void(std::size_t)> _disconnectionHandler;           ///< Function to handle client disconnections.
    std::mutex _mutex;                                                ///< Mutex to protect shared resources.
    std::unordered_map<size_t, std::shared_ptr<Session>> _session;    ///< Map of active sessions indexed by session ID.
};

} // namespace ntw
