//
// Created by Arthur Motelevicz on 04/03/23.
//

#ifndef BEASTBOYS_LOGGER_H
#define BEASTBOYS_LOGGER_H

#include <iostream>
#include "ErrorManager.h"

#define RETURN_IF_ASIO_ERROR_(ec) \
    if(ec.value() != 0) { \
      bb::ErrorInfo error(ec.value(), ec.message(), __FILE__, __LINE__); \
      bb::ErrorManager::getInstance().reportError(error); \
      return; \
    }

#define REPORT_ASIO_ERROR_(ec) \
    if(ec.value() != 0) { \
      bb::ErrorInfo error(ec.value(), ec.message(), __FILE__, __LINE__); \
      bb::ErrorManager::getInstance().reportError(error); \
    }


#endif //BEASTBOYS_LOGGER_H
