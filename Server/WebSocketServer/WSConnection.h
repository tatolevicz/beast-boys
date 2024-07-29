//
// Created by Arthur Motelevicz on 29/07/24.
//

#ifndef BEAST_BOYS_WSCONNECTION_H
#define BEAST_BOYS_WSCONNECTION_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "Connection.h"

namespace bb::network::server
{
  class ServerState;
}

namespace bb::network::server::ws
{

  class WSConnection : public bb::network::server::Connection, public std::enable_shared_from_this<WSConnection>
  {
  public:
    WSConnection(boost::beast::websocket::stream<boost::asio::ip::tcp::socket> sockStream, std::shared_ptr<bb::network::server::ServerState> serverState);
    virtual ~WSConnection() = default;

    virtual void run() override;
    virtual void send(const std::string& message) override;
    virtual void disconnect() override;
  protected:
    void callAsyncRead();
    void callAsyncWrite();
    void onRead(boost::system::error_code ec, std::size_t bytes);
    void onWrite(boost::system::error_code ec, std::size_t bytes);

    std::vector<std::string> _messageQueue;
    boost::beast::websocket::stream<boost::asio::ip::tcp::socket> _sockStream;
    boost::beast::flat_buffer _buffer;
  };
}
#endif //BEAST_BOYS_WSCONNECTION_H
