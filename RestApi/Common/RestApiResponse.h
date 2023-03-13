//
// Created by Arthur Motelevicz on 16/02/23.
//

#ifndef BROKERAPP_RESTAPIRESPONSE_H
#define BROKERAPP_RESTAPIRESPONSE_H

#include <string>

namespace bb {
    namespace network {
        namespace rest {

            struct Response {
                int http_result_code{-1};
                std::string message;
                std::string reply;

                Response(const std::string &msg = "", int http_result_code_value = -1);

                bool isOk() const;

            };

            template<typename T>
            struct DataResponse : Response {
                T data;

                DataResponse(const std::string &msg = "", int http_result_code_value = -1)
                        : Response(msg, http_result_code_value)
                {}
            };
        }
    }

}
#endif //BROKERAPP_RESTAPIRESPONSE_H
