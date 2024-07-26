//
// Created by Arthur Motelevicz on 26/02/23.
//

#ifndef SOCKET_SERVERSTATE_H
#define SOCKET_SERVERSTATE_H

#include <boost/asio.hpp>
#include <iostream>

namespace bb {
  namespace network::rs::server {
    class ServerState;

    class Client;

    class Connection;

    class ServerState : public std::enable_shared_from_this<ServerState> {
    public:
      ServerState();

      std::shared_ptr<Client> join(Connection *connection);

      void leave(Connection *connection);

      void send(const std::string &message);

      void leaveAll();

    private:
      std::vector<std::shared_ptr<Client>> _clients;

    };

  }
}

#endif //SOCKET_SERVERSTATE_H
