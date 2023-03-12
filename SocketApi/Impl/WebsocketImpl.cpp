//
// Created by Arthur Motelevicz on 01/03/23.
//

#include "WebsocketImpl.h"
#include <iostream>
#include "SharedState.h"
#include "Stream.h"
#include "Resolver.h"

namespace bb{
namespace network{
namespace ws{

using namespace boost;

WebsocketImpl::WebsocketImpl():
_sslContext(boost::asio::ssl::context::sslv23_client){
    _sharedState = std::make_shared<SharedState>();
    _sslContext.set_default_verify_paths();
    _sslContext.set_verify_mode(boost::asio::ssl::verify_peer);

    _work = std::make_shared<boost::asio::io_context::work>(_ioc);

    _worker = std::thread([&]() {
        while(!_destructorCalled) {
            try {
                _ioc.run();
            }
            catch (const boost::system::system_error &e) {
                REPORT_ASIO_ERROR_(e.code())
                restartContext();
            }
        }
    });
}

void WebsocketImpl::restartContext(){
    _work.reset();
    _ioc.restart();
    _work = std::make_shared<boost::asio::io_context::work>(_ioc);
}

std::shared_ptr<Stream>  WebsocketImpl::openStream(std::string baseUrl,
                              std::string port,
                              std::string endPoint,
                              bool usesSSL,
                              StreamCB cb){

    //safe check to defaults port and target
    if(port.empty())
        port = "443";

    if(endPoint.empty())
        endPoint = "/";

    std::shared_ptr<Stream> stream{nullptr};

    if(usesSSL) {
        stream = std::make_shared<Stream>(_ioc,
                                          std::move(baseUrl),
                                          std::move(port),
                                          std::move(endPoint),
                                          _sslContext,
                                          std::move(cb));
    }
    else {
        stream = std::make_shared<Stream>(_ioc,
                                          std::move(baseUrl),
                                          std::move(port),
                                          std::move(endPoint),
                                          std::move(cb));
    }


    std::make_shared<Resolver>(_sharedState, stream, _ioc)->run();

    return stream;
}

WebsocketImpl::~WebsocketImpl(){
    _destructorCalled = true;
    _ioc.stop();
    _worker.join();
}

}
}
}