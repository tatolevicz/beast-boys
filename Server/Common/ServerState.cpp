//
// Created by Arthur Motelevicz on 26/02/23.
//

#include "ServerState.h"
#include "Client.h"
#include "Connection.h"
#include "Logger.h"

namespace bb::network::server
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
  }

  void ServerState::send(const std::string &message)
  {
    for (const auto &c: _clients)
      c->getConnection()->send(message);

    if(_onSendMessageCb)
      _onSendMessageCb(message);
  }

  void ServerState::leaveAll()
  {
    for (const auto &c: _clients)
      c->getConnection()->disconnect();

    _clients.clear();
  }

  void ServerState::setOnSendMessageCB(const OnSendMessageCallback& cb)
  {
    _onSendMessageCb = cb;
  }

  ServerState::~ServerState()
  {
    if(!_clients.empty())
      lg(mgutils::Error) << "Clients not empty on ServerState destructor!\n";
  }

}
