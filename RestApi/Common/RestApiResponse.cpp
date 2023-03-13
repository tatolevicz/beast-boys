//
// Created by Arthur Motelevicz on 16/02/23.
//

#include "RestApiResponse.h"
namespace bb {
    namespace network {
        namespace rest {

            NetworkResponse::NetworkResponse(const std::string &msg, int http_result_code_value) :
                    message(msg),
                    http_result_code(http_result_code_value)
            {}

            bool NetworkResponse::isOk() const {
                return http_result_code >= 200 && http_result_code < 400;
            }
        }
    }
}