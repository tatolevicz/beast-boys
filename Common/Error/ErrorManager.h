//
// Created by Arthur Motelevicz on 28/07/24.
//

#ifndef BEAST_BOYS_ERRORMANAGER_H
#define BEAST_BOYS_ERRORMANAGER_H

#include "ErrorInfo.h"
#include <functional>
#include <vector>
#include <mutex>

namespace bb
{
class ErrorManager
{
public:
  using ErrorCallback = std::function<void(const ErrorInfo &)>;

  static ErrorManager &getInstance() {
    static ErrorManager instance;
    return instance;
  }

  void registerCallback(const ErrorCallback &callback) {
    std::lock_guard<std::mutex> lock(mutex_);
    callbacks.push_back(callback);
  }

  void reportError(const ErrorInfo &error) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto &callback: callbacks) {
      callback(error);
    }
  }

  ErrorManager(const ErrorManager &) = delete;
  ErrorManager &operator=(const ErrorManager &) = delete;
private:
  ErrorManager() = default;
  std::vector<ErrorCallback> callbacks;
  std::mutex mutex_;
};

}
#endif //BEAST_BOYS_ERRORMANAGER_H
