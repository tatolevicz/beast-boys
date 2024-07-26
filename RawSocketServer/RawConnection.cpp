//
// Created by Arthur Motelevicz on 26/02/23.
//

#include "RawConnection.h"
#include "ServerState.h"
#include "Logger.h"

//server beast

namespace bb
{
namespace network::rs::server
{
  Connection::Connection(boost::asio::ip::tcp::socket
  sock,
  std::shared_ptr<ServerState> serverState
  ):

  _socket(std::move(sock)),
  _serverState(std::move(serverState)) {

  }

  Connection::~Connection() {
    _serverState->leave(this);
  }

  void Connection::send(const std::string &message) {
    _messageQueue.push_back(message);
    callAsyncWrite();
  }

  void Connection::onRead(boost::system::error_code ec, std::size_t bytes) {

    CHECK_ASIO_ERROR_(ec)

    std::string message = boost::beast::buffers_to_string(_buffer.data());
    std::cout << "Msg: " << message << "\n";

    _buffer.consume(bytes);

    _serverState->send(message);

    callAsyncRead();
  }

  void Connection::callAsyncRead() {
//    _sockStream.async_read(_buffer, [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes) {
//      self->onRead(ec, bytes);
//    });

    boost::asio::async_read(_socket,_buffer,
                            boost::asio::transfer_at_least(1), // Read at least 1 byte to call the callback
    [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes)
    {
      self->onRead(ec, bytes);
    });
  }

  void Connection::callAsyncWrite() {
//    _sockStream.async_write(boost::asio::buffer(_messageQueue.front()),
//                            [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes) {
//                              self->onWrite(ec, bytes);
//                            });

    boost::asio::async_write(_socket, boost::asio::buffer(_messageQueue.front()),
    [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes_transferred)
    {
      self->onWrite(ec, bytes_transferred);
    });
  }


  void Connection::onWrite(boost::system::error_code ec, std::size_t bytes) {

    CHECK_ASIO_ERROR_(ec)
    _messageQueue.erase(_messageQueue.begin());

    if (!_messageQueue.empty()) {
      callAsyncWrite();
    }
  }

  void Connection::run()
  {
    _serverState->join(this);
    callAsyncRead();
  }

  void Connection::disconnect()
  {
    boost::asio::post(_socket.get_executor(),
    [this]()
    {
      boost::system::error_code ec;
      // Shutdown the socket to disallow further sends and receives
      _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
      CHECK_ASIO_ERROR_(ec)
      _socket.close(ec);
      CHECK_ASIO_ERROR_(ec)
    });
  }
}
}
