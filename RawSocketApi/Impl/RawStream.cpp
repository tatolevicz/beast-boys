//
// Created by Arthur Motelevicz on 04/03/23.
//

#include "RawStream.h"
#include <iostream>
#include <utility>
#include "Logger.h"

namespace bb::network::rs
{

Stream::Stream(boost::asio::io_context &ioc,
               std::string host,
               std::string port,
               std::string target,
               StreamCB cb,
               char delimiter):
_host(std::move(host)),
_port(std::move(port)),
_target(std::move(target)),
_cb(std::move(cb)),
_readUntilDelimiter(delimiter)
{
  _id = ++id;
  _socket = std::make_shared<boost::asio::ip::tcp::socket>(ioc);
}

Stream::Stream(boost::asio::io_context &ioc,
               std::string host,
               std::string port,
               std::string target,
               StreamCB2 cb,
               char delimiter):
    _host(std::move(host)),
    _port(std::move(port)),
    _target(std::move(target)),
    _cb2(std::move(cb)),
    _readUntilDelimiter(delimiter)

{
  _id = ++id;
  _socket = std::make_shared<boost::asio::ip::tcp::socket>(ioc);
}

uint32_t Stream::getId() const
{
  return _id;
}

std::string Stream::host() const
{
  return _host;
}

std::string Stream::port() const
{
  return _port;
}

std::string Stream::target() const
{
  return _target;
}

boost::asio::ip::tcp::socket& Stream::getSocket()
{
  return *_socket;
}

void Stream::feedData(const std::string& data)
{
  if(_cb)
    _cb(true, data, shared_from_this());
}

void Stream::feedData(const char* data, size_t length)
{
  if(_cb2)
  {
    _cb2(true, data, length, shared_from_this());
    return;
  }

  feedData(std::string(data, length));
}


void Stream::connectionAborted(boost::system::error_code ec){
  if(_cb)
    _cb(false, ec.message(), shared_from_this());
}

void Stream::internalStop()
{
  if (_socket && _socket->is_open())
  {
//    _socket->async_close(boost::beast::websocket::close_code::normal, [&](boost::system::error_code ec) {
//        if(_wasClosedByServer) return;
//        lg(mgutils::Info) << "Stream " << id << " stopped by user!\n";
//    });

    boost::asio::post(_socket->get_executor(),
    [this]()
    {
      boost::system::error_code ec;

      // Shutdown the socket to disallow further sends and receives
      _socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

      RETURN_IF_ASIO_ERROR_(ec)

      _socket->close(ec);

      RETURN_IF_ASIO_ERROR_(ec)

      if(_wasClosedByServer)
        return;

      lg(mgutils::Info)<< "Stream " << id << " stopped by user!\n";
    });
  }
}

void Stream::stop() {
  _wasClosedByClient = true;
  internalStop();
}

void Stream::stopWithCloseCallbackTriggered() {
  stop();
  if(_closeStreamCB)
    _closeStreamCB(shared_from_this());
}

//void Stream::ping(const std::string& payload) {
//
//    auto pingCb = [&](boost::system::error_code ec) {
//        REPORT_ASIO_ERROR_(ec)
//    };
//
////    _socket->async_ping(payload.c_str(),pingCb);
//}

Stream::~Stream()
{
  lg(mgutils::Debug) << "Destructor stream!";
}


void Stream::setCloseStreamCallback(const CloseStreamCallback& cb)
{
  _closeStreamCB = cb;
}

void Stream::setPongStreamCallback(const PongStreamCallback& cb)
{
  _pongStreamCB = cb;
}

void Stream::setPingStreamCallback(const PingStreamCallback& cb)
{
  _pingStreamCB = cb;
}

bool Stream::wasClosedByServer() const
{
  return _wasClosedByServer;
}

bool Stream::wasClosedByClient() const
{
  return _wasClosedByClient;
}

bool Stream::isOpen() const
{
  return _socket->is_open();
}

char Stream::getReadUntilDelimiter() const
{
  return _readUntilDelimiter;
}

}