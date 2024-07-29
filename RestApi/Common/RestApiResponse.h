//
// Created by Arthur Motelevicz on 16/02/23.
//

#ifndef BROKERAPP_RESTAPIRESPONSE_H
#define BROKERAPP_RESTAPIRESPONSE_H

#include <string>

namespace bb::network::rest
{
  struct NetworkResponse
  {
    int http_result_code{-1};
    std::string message;
    std::string data;
    explicit NetworkResponse(const std::string &msg = "", int http_result_code_value = -1);
    bool isOk() const;
  };

}
#endif //BROKERAPP_RESTAPIRESPONSE_H
