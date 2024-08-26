//
// Created by Arthur Motelevicz on 28/07/24.
//

#ifndef BEAST_BOYS_TESTHELPERS_H
#define BEAST_BOYS_TESTHELPERS_H

#include <beastboys>
#include <string>
#include <mgutils/ErrorManager.h>

void errorCallback(const mgutils::ErrorInfo& error);
void infoCallback(const mgutils::ErrorInfo& error);
void handleServerMessages(bb::network::server::Server& server, const std::string& message);

template<typename Condition>

void waitForCondition(Condition condition, int attempts = 200, int delayMs = 50)
{
  for (int i = 0; i < attempts; ++i) {
    if (condition()) {
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
  }
}

#endif //BEAST_BOYS_TESTHELPERS_H
