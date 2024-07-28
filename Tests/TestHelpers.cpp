//
// Created by Arthur Motelevicz on 28/07/24.
//

#include "TestHelpers.h"

void errorCallback(const bb::ErrorInfo& error)
{
  std::cerr << "Error Code: " << error.errorCode
            << " | Error Message: " << error.errorMessage
            << " | File: " << error.file << "(" << error.line << ")\n";
}

void handleServerMessages(bb::network::rs::server::RawServer& server, const std::string& message)
{
  if (message.empty())
    return;

  if (message == "stop") {
    server.stop();
  } else if (message == "close")
  {
    server.disconnectAll();
  }
}