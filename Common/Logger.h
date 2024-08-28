//
// Created by Arthur Motelevicz on 04/03/23.
//

#ifndef BEASTBOYS_LOGGER_H
#define BEASTBOYS_LOGGER_H

#include <mgutils/Logger.h>

#define lg mgutils::Logger::instance().log

class Logger {
public:
    static Logger& instance()
    {
        static Logger instance;
        return instance;
    }

    void logDebug(const std::string& message)
    {
      lg(mgutils::Debug, message);
    }

    void logInfo(const std::string& message)
    {
      lg(mgutils::Info, message);
    }

    void logWarning(const std::string& message)
    {
      lg(mgutils::Warning, message);
    }

    void logError(const std::string& message)
    {
      lg(mgutils::Error, message);
    }

private:
    Logger() = default;
    ~Logger() = default;

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::mutex mutex_;
};


#define LOG_DEBUG(message) Logger::instance().logDebug(message)
#define LOG_INFO(message) Logger::instance().logInfo(message)

#define LOG_WARNING(message) Logger::instance().logWarning(message)
#define LOG_ERROR(message) Logger::instance().logError(message)

#define RETURN_IF_ASIO_ERROR_(ec) \
  if (ec.value() != 0) { \
      NOTIFY_ERROR(ec.value(), ec.message()); \
      return; \
  }

#define REPORT_ASIO_ERROR_(ec) \
  if (ec.value() != 0) { \
    NOTIFY_ERROR(ec.value(), ec.message()); \
  }

#endif // BEASTBOYS_LOGGER_H