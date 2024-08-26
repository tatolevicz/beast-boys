//
// Created by Arthur Motelevicz on 28/07/24.
//

#include "TestHelpers.h"

void errorCallback(const mgutils::ErrorInfo& error)
{
  std::stringstream stm;
  stm << "Error Code: " << error.errorCode
      << " | Error Message: " << error.errorMessage
      << " | File: " << error.file << "(" << error.line << ")";

  LOG_ERROR(stm.str());
}

void infoCallback(const mgutils::ErrorInfo& error)
{
  std::stringstream stm;
  stm << "Error Code: " << error.errorCode
      << " | Error Message: " << error.errorMessage
      << " | File: " << error.file << "(" << error.line << ")";

  LOG_INFO(stm.str());
}

void handleServerMessages(bb::network::server::Server& server, const std::string& message)
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