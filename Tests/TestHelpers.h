//
// Created by Arthur Motelevicz on 28/07/24.
//

#ifndef BEAST_BOYS_TESTHELPERS_H
#define BEAST_BOYS_TESTHELPERS_H

#include <beastboys>
#include <string>

void handleServerMessages(bb::network::rs::server::RawServer& server, const std::string& message);

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
