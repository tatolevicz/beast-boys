//
// Created by Arthur Motelevicz on 04/03/23.
//

#include "RawSharedState.h"

namespace bb {
namespace network {
namespace rs {

void SharedState::join(Stream *s){
    _streams.insert(s);
}

void SharedState::leave(Stream *s){
    _streams.erase(s);
}


}
}
}