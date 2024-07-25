//
// Created by Arthur Motelevicz on 24/07/24.
//

#ifndef BEAST_BOYS_RAWSOCKETIMPL_H
#define BEAST_BOYS_RAWSOCKETIMPL_H

#include <boost/asio.hpp>
#include <memory>
#include <string>

class RawSocketImpl : public std::enable_shared_from_this<RawSocketImpl> {
public:
  explicit RawSocketImpl(boost::asio::io_context& ioc);

  void connect(const std::string& host, const std::string& port);
  void send(const std::string& message);
  void close();

private:
  void on_connect(boost::system::error_code ec, const boost::asio::ip::tcp::resolver::iterator& endpoint_it);
  void do_read();
  void on_read(boost::system::error_code ec, std::size_t bytes_transferred);
  static void on_write(boost::system::error_code ec, std::size_t bytes_transferred);

  boost::asio::ip::tcp::socket _socket;
  boost::asio::streambuf _buffer;
  bool _is_connected;
  boost::asio::io_context* _ioc{nullptr};
};

#endif //BEAST_BOYS_RAWSOCKETIMPL_H
