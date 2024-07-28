//
// Created by Arthur Motelevicz on 04/03/23.
//

#ifndef BEASTBOYS_LOGGER_H
#define BEASTBOYS_LOGGER_H

#include <iostream>

#define CHECK_ASIO_ERROR_(ec) \
    if(ec.value() != 0) { \
        std::cerr << "Error Code: " << ec.value() \
                  << " | Error Number: " << std::generic_category().default_error_condition(ec.value()).value() \
                  << " | Error Message: " << ec.message()                                                       \
                  << " | Default Error Message: " << ec.default_error_condition().message() \
                  << " | File: " << __FILE__ << "(" << __LINE__ << ")\n"; \
        return; \
    }

#define REPORT_ASIO_ERROR_(ec) \
    if(ec.value() != 0) { \
        std::cerr << "Error Code: " << ec.value() \
                  << " | Error Number: " << std::generic_category().default_error_condition(ec.value()).value() \
                  << " | Error Message: " << ec.message()                                                       \
                  << " | Default Error Message: " << ec.default_error_condition().message() \
                  << " | File: " << __FILE__ << "(" << __LINE__ << ")\n"; \
    }


#endif //BEASTBOYS_LOGGER_H
