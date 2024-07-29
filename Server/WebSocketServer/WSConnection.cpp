//
// Created by Arthur Motelevicz on 29/07/24.
//

#include "WSConnection.h"
#include "Logger.h"
#include "ServerState.h"

namespace bb::network::server::ws
{
  WSConnection::WSConnection(boost::beast::websocket::stream<boost::asio::ip::tcp::socket> sockStream, std::shared_ptr<bb::network::server::ServerState> serverState):
  bb::network::server::Connection(std::move(serverState)),
  _sockStream(std::move(sockStream))
  {

  }

  void WSConnection::send(const std::string& message){
    _messageQueue.push_back(message);
    callAsyncWrite();
  }

  void WSConnection::onRead(boost::system::error_code ec, std::size_t bytes){

    RETURN_IF_ASIO_ERROR_(ec)
    if (bytes > 0)
    {
      std::string message = boost::beast::buffers_to_string(_buffer.data());
      LOG_INFO(message);
      _buffer.consume(bytes);
      _serverState->send(message);
    }

    callAsyncRead();
  }

  void WSConnection::callAsyncRead() {
    _sockStream.async_read(_buffer,
    [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes)
    {
      self->onRead(ec, bytes);
    });
  }

  void WSConnection::callAsyncWrite()
  {
    _sockStream.async_write(boost::asio::buffer(_messageQueue.front()),
    [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes)
    {
      self->onWrite(ec, bytes);
    });
  }
//
//  void WSConnection::onHandShake(boost::system::error_code ec){
//
//    RETURN_IF_ASIO_ERROR_(ec)
//    _serverState->join(this);
//    callAsyncRead();
//  }

  void WSConnection::onWrite(boost::system::error_code ec, std::size_t bytes){

    RETURN_IF_ASIO_ERROR_(ec)
    _messageQueue.erase(_messageQueue.begin());

    if(!_messageQueue.empty()){
      callAsyncWrite();
    }
  }

  void WSConnection::run()
  {
//    _sockStream.async_accept(_req, [self = shared_from_this()](boost::system::error_code ec){
//      self->onHandShake(ec);
//    });
    _serverState->join(this);
    callAsyncRead();
  }

  void WSConnection::disconnect()
  {
    _sockStream.async_close(boost::beast::websocket::normal,[](boost::system::error_code ec){
      RETURN_IF_ASIO_ERROR_(ec)
    });
  }
}