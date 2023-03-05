//
// Created by Arthur Motelevicz on 04/03/23.
//

#include "SharedState.h"

namespace bb {
namespace network {
namespace ws {

void SharedState::join(Stream *s){
    _streams.insert(s);
}

void SharedState::leave(Stream *s){
    _streams.erase(s);
}


}
}
}