//
// Created by Arthur Motelevicz on 04/03/23.
//

#ifndef RAW_CONNECTOR_H
#define RAW_CONNECTOR_H

#include "RawSharedState.h"

namespace bb {
namespace network {
namespace rs {

class Connector : public std::enable_shared_from_this<Connector> {
public:
    Connector(std::shared_ptr<SharedState> state, std::shared_ptr<Stream> stream);
    void run(const boost::asio::ip::tcp::resolver::results_type& results);
    void onConnect(boost::system::error_code ec, boost::asio::ip::tcp::resolver::iterator);


private:

    std::shared_ptr<Stream>  _stream;
    std::shared_ptr<SharedState> _sharedState{nullptr};

};

}
}
}

#endif //RAW_CONNECTOR_H
