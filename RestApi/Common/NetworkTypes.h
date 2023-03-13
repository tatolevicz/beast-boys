//
// Created by Arthur Motelevicz on 16/02/23.
//

#ifndef BROKERAPP_NETWORKTYPES_H
#define BROKERAPP_NETWORKTYPES_H

#include "Json.h"
#include <boost/variant.hpp>
#include <boost/beast.hpp>
#include "RestApiResponse.h"

namespace bb {
    namespace network {
        namespace rest {

            enum TaskExecutionType {
                SYNCH,
                ASYNCH
            };

            enum ContentType {
                FORM,
                JSON
            };

            using NetworkResponse = DataResponse<std::string>;
            using PostCallback = std::function<bool(const char *fl, int ec, std::string errmsg, std::string res,
                                                    int http_result_code)>;
            using ResponseCallback = std::function<void(const NetworkResponse &)>;
            using valType = boost::variant<std::size_t, const char *, std::string>;
            using kvType = std::pair<std::string, std::string>;
            using ListData = std::vector<kvType>;
            using RequestType = boost::beast::http::verb;
            using HeaderType = boost::beast::http::field;
            using HeaderVariant = std::variant<HeaderType, std::string>;
        }
    }
}
#endif //BROKERAPP_NETWORKTYPES_H
