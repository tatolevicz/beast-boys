//
// Created by Arthur Motelevicz on 01/03/23.
//

#ifndef BINANCEBEAST_WEBSOCKETIMPL_H
#define BINANCEBEAST_WEBSOCKETIMPL_H

#include <memory>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include "WebsocketTypes.h"

namespace bb{
namespace network{
namespace ws{

class Stream;
class SharedState;

class WebsocketImpl{
public:
    WebsocketImpl();
    std::weak_ptr<Stream> openStream(std::string baseUrl,
                    std::string port,
                    std::string endPoint,
                    bool usesSSL,
                    StreamCB cb);

    ~WebsocketImpl();
    boost::asio::io_context _ioc;
    boost::asio::ssl::context _sslContext;
    std::shared_ptr<SharedState> _sharedState{nullptr};
    std::thread _worker;
    std::shared_ptr<boost::asio::io_context::work> _work{nullptr};
    bool _destructorCalled = false;

private:
    void startContext();
    void restartContext();

};

}
}
}

#endif //BINANCEBEAST_WEBSOCKETIMPL_H
