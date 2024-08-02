//
// Created by Arthur Motelevicz on 04/03/23.
//

#ifndef BEASTBOYS_LOGGER_H
#define BEASTBOYS_LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include "ErrorManager.h"

#define RESET       "\033[0m"
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

#define BOLD_BLACK   "\033[1m\033[30m"
#define BOLD_RED     "\033[1m\033[31m"
#define BOLD_GREEN   "\033[1m\033[32m"
#define BOLD_YELLOW  "\033[1m\033[33m"
#define BOLD_BLUE    "\033[1m\033[34m"
#define BOLD_MAGENTA "\033[1m\033[35m"
#define BOLD_CYAN    "\033[1m\033[36m"
#define BOLD_WHITE   "\033[1m\033[37m"

#define DARK_GRAY "\033[38;5;8m"


class Logger {
public:
    static Logger& instance()
    {
        static Logger instance;
        return instance;
    }

    void logDebug(const std::string& message)
    {
        #ifdef DEBUG
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << BOLD_BLUE << "DEBUG: " << message << RESET << "\n";
        #endif
    }

    void logInfo(const std::string& message)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << DARK_GRAY << "INFO: " << message << RESET << "\n";
    }

    void logWarning(const std::string& message)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cerr << BOLD_YELLOW << "WARNING: " << message << RESET << "\n";
    }

    void logError(const std::string& message)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cerr << BOLD_RED << "ERROR: " << message << RESET << "\n";
    }

    void logToFile(const std::string& message)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::ofstream logfile("log.txt", std::ios_base::app);
        logfile << message << "\n";
    }

private:
    Logger() = default;
    ~Logger() = default;

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::mutex mutex_;
};

#ifdef DEBUG
#define LOG_DEBUG(message) Logger::instance().logDebug(message)
#else
#define LOG_DEBUG(message) // Do nothing in release mode
#endif

#ifdef DEBUG
#define LOG_INFO(message) Logger::instance().logInfo(message)
#else
#define LOG_INFO(message) Logger::instance().logToFile(message)
#endif

#ifdef DEBUG
#define LOG_WARN(message) Logger::instance().logWarning(message)
#else
#define LOG_WARN(message) Logger::instance().logToFile(message)
#endif

#define LOG_WARNING(message) Logger::instance().logWarning(message)
#define LOG_ERROR(message) Logger::instance().logError(message)

#define RETURN_IF_ASIO_ERROR_(ec) \
  if (ec.value() != 0) { \
      ErrorInfo error(ec.value(), ec.message(), __FILE__, __LINE__); \
      ErrorManager::instance().notify(error); \
      return; \
  }

#define REPORT_ASIO_ERROR_(ec) \
  if (ec.value() != 0) { \
      ErrorInfo error(ec.value(), ec.message(), __FILE__, __LINE__); \
      ErrorManager::instance().notify(error); \
  }

#endif // BEASTBOYS_LOGGER_H