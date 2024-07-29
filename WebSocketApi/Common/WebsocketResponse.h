//
// Created by Arthur Motelevicz on 01/03/23.
//

#ifndef BINANCEBEAST_WEBSOCKETRESPONSE_H
#define BINANCEBEAST_WEBSOCKETRESPONSE_H

#include <string>

namespace bb {
namespace network {
namespace ws {

struct Response {
    int code{0};
    std::string message;

    Response(const int pCode = 0, const std::string &msg = "");
    bool isOk() const;

};

template<typename T>
struct DataResponse : Response {
    T data;

    DataResponse(const int pCode = 0, const std::string &msg = "")
            : Response(
            pCode, msg) {}
};

}
}
}

#endif //BINANCEBEAST_WEBSOCKETRESPONSE_H

