//
// Created by Arthur Motelevicz on 01/03/23.
//

#ifndef RAW_SOCKETIMPL_H
#define RAW_SOCKETIMPL_H

#include <memory>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include "RawSocketTypes.h"

namespace bb{
namespace network{
namespace rs{

class Stream;
class SharedState;

class RawSocketImpl{
public:
  RawSocketImpl();
    std::weak_ptr<Stream> openStream( std::string baseUrl,
                                      std::string port,
                                      std::string endPoint,
                                      StreamCB cb,
                                      char delimiter = '\0');

    std::weak_ptr<Stream> openStream(std::string baseUrl,
                                   std::string port,
                                   std::string endPoint,
                                   StreamCB2 cb,
                                   char delimiter = '\0');

    ~RawSocketImpl();
    boost::asio::io_context _ioc;
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

#endif //RAW_SOCKETIMPL_H
