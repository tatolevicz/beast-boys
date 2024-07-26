//
// Created by Arthur Motelevicz on 26/02/23.
//

#ifndef SOCKET_WEBSOCKETCONNECTION_H
#define SOCKET_WEBSOCKETCONNECTION_H

#include <boost/asio.hpp>

namespace bb::network::rs::server
{
  class ServerState;

  class Connection : public std::enable_shared_from_this<Connection>
  {
  public:
    Connection(boost::asio::ip::tcp::socket sock,
                          std::shared_ptr<ServerState> serverState);

      ~Connection();

      void run();
      void send(const std::string& message);
      void disconnect();
  private:
      void callAsyncRead();
      void callAsyncWrite();
      void onRead(boost::system::error_code ec, std::size_t bytes);
      void onWrite(boost::system::error_code ec, std::size_t bytes);

      std::shared_ptr<ServerState> _serverState{nullptr};
      boost::asio::ip::tcp::socket _socket;
      boost::asio::streambuf _buffer;
      std::vector<std::string> _messageQueue;
  };
}


#endif //SOCKET_WEBSOCKETCONNECTION_H
