////
//// Created by Arthur Motelevicz on 29/07/24.
////
//
#include "Connection.h"
#include "ServerState.h"
//#include "Logger.h"
//
//#define BUFFER_SIZE 2048
//

namespace bb::network::server
{
  Connection::Connection(std::shared_ptr <ServerState> serverState):
  _serverState(std::move(serverState))
  {}

  Connection::~Connection() {
    _serverState->leave(this);
  }
}

//
//  void Connection::send(const std::string &message)
//  {
//    _messageQueue.push_back(message);
//    callAsyncWrite();
//  }
//
//  void Connection::onRead(boost::system::error_code ec, std::size_t bytes)
//  {
//    RETURN_IF_ASIO_ERROR_(ec)
//    if (bytes > 0)
//    {
//      _buffer.commit(bytes);
//      std::istream is(&_buffer);
//      std::string msg((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
//
//      std::stringstream stm;
//      stm << "Bytes: " << bytes << " Msg: " << msg;
//      LOG_INFO(stm.str());
//
//      _buffer.consume(bytes);
//      _serverState->send(msg);
//    }
//
//    callAsyncRead();
//  }
//
//  void Connection::callAsyncRead()
//  {
//
//    _socket.async_read_some(_buffer.prepare(BUFFER_SIZE),
//                            [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes)
//                            {
//                              self->onRead(ec, bytes);
//                            });
////    boost::asio::async_read(
////      _socket,_buffer,
////      boost::asio::transfer_at_least(1), // Read at least 1 byte to call the callback
////      [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes)
////      {
////        self->onRead(ec, bytes);
////      });
//  }
//
//  void Connection::callAsyncWrite()
//  {
//    boost::asio::async_write(_socket, boost::asio::buffer(_messageQueue.front()),
//                             [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes_transferred)
//                             {
//                               self->onWrite(ec, bytes_transferred);
//                             });
//  }
//
//  void Connection::onWrite(boost::system::error_code ec, std::size_t bytes)
//  {
//    RETURN_IF_ASIO_ERROR_(ec)
//    _messageQueue.erase(_messageQueue.begin());
//
//    if (!_messageQueue.empty())
//    {
//      callAsyncWrite();
//    }
//  }
//
//  void Connection::run()
//  {
//    _serverState->join(this);
//    callAsyncRead();
//  }
//
//  void Connection::disconnect()
//  {
//    boost::asio::post(_socket.get_executor(),
//    [this]()
//    {
//      boost::system::error_code ec;
//      // Shutdown the socket to disallow further sends and receives
//      _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
//      RETURN_IF_ASIO_ERROR_(ec)
//      _socket.close(ec);
//      RETURN_IF_ASIO_ERROR_(ec)
//    });
//  }
//}