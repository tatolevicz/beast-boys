//
// Created by Arthur Motelevicz on 04/03/23.
//

#ifndef BINANCEBEAST_TARGETHANDSHAKER_H
#define BINANCEBEAST_TARGETHANDSHAKER_H


#include "SharedState.h"

namespace bb {
namespace network {
namespace ws {

class TargetHandShaker : public std::enable_shared_from_this<TargetHandShaker> {
public:
    TargetHandShaker(std::shared_ptr<SharedState> state, std::shared_ptr<Stream> stream);
    void run();
    void onHandShake(boost::system::error_code ec);
private:
    std::shared_ptr<Stream> _stream;
    std::shared_ptr<SharedState> _sharedState{nullptr};
};

}
}
}

#endif //BINANCEBEAST_TARGETHANDSHAKER_H
