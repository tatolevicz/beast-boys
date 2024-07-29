//
// Created by Arthur Motelevicz on 26/02/23.
//

#ifndef SOCKET_SERVERSTATE_H
#define SOCKET_SERVERSTATE_H

#include <boost/asio.hpp>
#include <iostream>

namespace bb::network::server
{
  class ServerState;
  class Client;
  class Connection;

  using OnSendMessageCallback = std::function<void(const std::string& message)>;

  class ServerState : public std::enable_shared_from_this<ServerState>
  {
  public:
    ServerState() = default;
    ~ServerState();

    std::shared_ptr<Client> join(Connection *connection);
    void leave(Connection *connection);
    void send(const std::string &message);
    void leaveAll();
    void setOnSendMessageCB(const OnSendMessageCallback& cb);

  private:
    OnSendMessageCallback _onSendMessageCb{nullptr};
    std::vector<std::shared_ptr<Client>> _clients;
  };
}


#endif //SOCKET_SERVERSTATE_H
