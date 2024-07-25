//
// Created by Arthur Motelevicz on 24/07/24.
//

#include "RawSocketImpl.h"
#include <iostream>
#include <boost/asio.hpp>


RawSocketImpl::RawSocketImpl(boost::asio::io_context& ioc):
_ioc(&ioc) ,_socket(ioc), _is_connected(false)
{}

void RawSocketImpl::connect(const std::string& host, const std::string& port)
{
  auto self(shared_from_this());

  boost::asio::ip::tcp::resolver::query resolver_query(host,port,boost::asio::ip::tcp::resolver::numeric_service);

  boost::asio::ip::tcp::resolver resolver(*_ioc);
  boost::system::error_code ec;

  boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(resolver_query, ec);

  boost::asio::connect(_socket,it,ec);

  on_connect(ec, {});
}

void RawSocketImpl::on_connect(boost::system::error_code ec, const boost::asio::ip::tcp::resolver::iterator& it)
{
  if (ec)
  {
    std::cerr << "Connect error: " << ec.message() << std::endl;
    return;
  }

  _is_connected = true;
  do_read();
}

void RawSocketImpl::send(const std::string& message)
{
  if (!_is_connected) {
    std::cerr << "Not connected" << std::endl;
    return;
  }
  auto self(shared_from_this());
  boost::asio::async_write(_socket, boost::asio::buffer(message + "\n"),
  [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
   on_write(ec, bytes_transferred);
  });
}

void RawSocketImpl::do_read()
{
  auto self(shared_from_this());
  boost::asio::async_read_until(_socket, _buffer, '\n',
  [this, self](boost::system::error_code ec, std::size_t bytes_transferred)
  {
    on_read(ec, bytes_transferred);
  });
}

void RawSocketImpl::on_read(boost::system::error_code ec, std::size_t bytes_transferred)
{
  if (ec)
  {
    std::cerr << "Read error: " << ec.message() << std::endl;
    return;
  }

  std::istream is(&_buffer);
  std::string response;
  std::getline(is, response);

  std::cout << "Received: " << response << std::endl;

  // Continue reading
  do_read();
}

void RawSocketImpl::on_write(boost::system::error_code ec, std::size_t bytes_transferred)
{
  if (ec)
  {
    std::cerr << "Write error: " << ec.message() << std::endl;
    return;
  }

  // Optionally handle post-write operations
}

void RawSocketImpl::close()
{
  if (_is_connected)
  {
    boost::system::error_code ec;
    _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    _socket.close(ec);
    if (ec)
    {
      std::cerr << "Close error: " << ec.message() << std::endl;
    }

    _is_connected = false;
  }
}
