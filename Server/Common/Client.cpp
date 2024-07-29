//
// Created by Arthur Motelevicz on 26/02/23.
//

#include "Client.h"
#include "ServerState.h"
#include "Connection.h"

namespace bb::network::server
{
  Client::Client(Connection *connection):
  _connection(connection)
  {
    _id = ++idCounter;
  }

  unsigned long Client::getId() const
  {
    return _id;
  }

  Connection *Client::getConnection()
  {
    return _connection;
  }
}


