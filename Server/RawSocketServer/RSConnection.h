//
// Created by Arthur Motelevicz on 29/07/24.
//

#ifndef BEAST_BOYS_RSCONNECTION_H
#define BEAST_BOYS_RSCONNECTION_H

#include <boost/asio.hpp>
#include "Connection.h"

namespace bb::network::server
{
  class ServerState;
}

namespace bb::network::server::rs
{

  class RSConnection : public bb::network::server::Connection, public std::enable_shared_from_this<RSConnection>
  {
  public:
    RSConnection(boost::asio::ip::tcp::socket sock, std::shared_ptr<bb::network::server::ServerState> serverState);
    virtual ~RSConnection() = default;

    virtual void run() override;
    virtual void send(const std::string& message) override;
    virtual void disconnect() override;
  protected:
    void callAsyncRead();
    void callAsyncWrite();
    void onRead(boost::system::error_code ec, std::size_t bytes);
    void onWrite(boost::system::error_code ec, std::size_t bytes);

    boost::asio::streambuf _buffer;
    std::vector<std::string> _messageQueue;
    boost::asio::ip::tcp::socket _socket;

  };
}

#endif //BEAST_BOYS_RSCONNECTION_H
