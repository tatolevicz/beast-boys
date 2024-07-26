//
// Created by Arthur Motelevicz on 26/02/23.
//

#include "Client.h"
#include "ServerState.h"
#include "RawConnection.h"

namespace bb {
namespace network::rs::server
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
}

