//
// Created by Arthur Motelevicz on 04/03/23.
//

#ifndef BINANCEBEAST_SSLHANDSHAKER_H
#define BINANCEBEAST_SSLHANDSHAKER_H


#include "SharedState.h"

namespace bb {
namespace network {
namespace ws {

class SSLHandShaker : public std::enable_shared_from_this<SSLHandShaker> {
public:
    SSLHandShaker(std::shared_ptr<SharedState> state, std::shared_ptr<Stream> stream);
    void run();
    void onHandShake(boost::system::error_code ec);
private:
    std::shared_ptr<Stream> _stream;
    std::shared_ptr<SharedState> _sharedState{nullptr};
};

}
}
}

#endif //BINANCEBEAST_SSLHANDSHAKER_H
