//
// Created by Arthur Motelevicz on 04/03/23.
//

#ifndef BINANCEBEAST_LOGGER_H
#define BINANCEBEAST_LOGGER_H

#include <iostream>

#define CHECK_ASIO_ERROR_(ec) \
    if(ec.value() != 0) { \
        std::cerr << "Error: " << ec.value() \
        << " at " << __FILE__ \
        << "(" << __LINE__ << ")"            \
        << " Message: " << ec.message() << "\n\n"; \
        return; \
    }

#define REPORT_ASIO_ERROR_(ec) \
    if(ec.value() != 0) { \
        std::cerr << "Error: " << ec.value() \
        << " at " << __FILE__ \
        << "(" << __LINE__ << ")"            \
        << " Message: " << ec.message() << "\n\n"; \
    }


#endif //BINANCEBEAST_LOGGER_H
