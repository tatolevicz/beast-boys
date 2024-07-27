//
// Created by Arthur Motelevicz on 26/02/23.
//

#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <boost/asio.hpp>

namespace bb::network::rs
{
  class Stream;
}

namespace bb::network::rs::server
{
  class ServerState;
  class Doorman;


  using OnSendMessageCallback = std::function<void(const std::string& message)>;

  class RawServer : public std::enable_shared_from_this<RawServer> {
  public:
    RawServer() = default;
    void start(uint32_t port);
    void stop();
    void disconnectAll();
    void setOnSendMessageCB(const OnSendMessageCallback& cb);
  private:
    OnSendMessageCallback _onSendMessageCb{nullptr};
    std::shared_ptr<ServerState> _serverState{nullptr};
    std::shared_ptr<Doorman> _doorMan{nullptr};
    boost::asio::io_context _ioc;
    boost::asio::ip::tcp::endpoint _endpoint;
  };
}



#endif //SOCKET_SERVER_H
