//
// Created by Arthur Motelevicz on 26/02/23.
//

#include "ServerState.h"
#include "Client.h"
#include "RawConnection.h"
#include "Logger.h"

namespace bb {
namespace network::rs::server
{
  std::shared_ptr<Client> ServerState::join(Connection *connection)
  {
    auto client = std::make_shared<Client>(connection);
    _clients.push_back(client);
    return client;
  }

  void ServerState::leave(Connection *connection)
  {
    auto it = std::find_if(_clients.begin(), _clients.end(), [&](std::shared_ptr<Client> &client) {
      return client->getConnection() == connection;
    });

    if (it != _clients.end())
      _clients.erase(it);
    else
      std::cerr << "Client not found to delete!\n";
  }

  void ServerState::send(const std::string &message)
  {
    for (auto c: _clients)
      c->getConnection()->send(message);
  }

  void ServerState::leaveAll()
  {
    for (auto c: _clients)
      c->getConnection()->disconnect();

    _clients.clear();
  }
}
}
