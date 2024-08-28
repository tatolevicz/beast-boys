//
// Created by Arthur Motelevicz on 01/03/23.
//

#include "RawSocketImpl.h"
#include <iostream>
#include "RawSharedState.h"
#include "RawStream.h"
#include "RawResolver.h"

#ifdef __APPLE__
#include <pthread.h>
#endif

namespace bb{
namespace network{
namespace rs{

using namespace boost;

  RawSocketImpl::RawSocketImpl()
  {
    _sharedState = std::make_shared<SharedState>();
    startContext();
}

void RawSocketImpl::startContext()
{
  _work = std::make_shared<boost::asio::io_context::work>(_ioc);
  _worker = std::thread([&]()
  {

#ifdef __APPLE__
    pthread_setname_np("RawSockApi-Worker");
#endif

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


void RawSocketImpl::restartContext(){
    _work.reset();
    _ioc.restart();
    startContext();
}

std::weak_ptr<Stream>  RawSocketImpl::openStream(std::string baseUrl,
                              std::string port,
                              std::string endPoint,
                              StreamCB cb,
                             char delimiter)
{
    //safe check to defaults port and target
    if(port.empty())
        port = "443";

    if(endPoint.empty())
        endPoint = "/";

    std::shared_ptr<Stream> stream{nullptr};


    stream = std::make_shared<Stream>(_ioc,
                                      std::move(baseUrl),
                                      std::move(port),
                                      std::move(endPoint),
                                      std::move(cb),
                                      delimiter);



    std::make_shared<Resolver>(_sharedState, stream, _ioc)->run();

    return stream;
}

  std::weak_ptr<Stream>  RawSocketImpl::openStream(std::string baseUrl,
                                                   std::string port,
                                                   std::string endPoint,
                                                   StreamCB2 cb,
                                                   char delimiter)
  {

    //safe check to defaults port and target
    if(port.empty())
      port = "443";

    if(endPoint.empty())
      endPoint = "/";

    std::shared_ptr<Stream> stream{nullptr};


    stream = std::make_shared<Stream>(_ioc,
                                      std::move(baseUrl),
                                      std::move(port),
                                      std::move(endPoint),
                                      std::move(cb),
                                      delimiter);



    std::make_shared<Resolver>(_sharedState, stream, _ioc)->run();

    return stream;
  }

RawSocketImpl::~RawSocketImpl(){
  _destructorCalled = true;
  _ioc.stop();
  _work.reset();
  if(_worker.joinable())
      _worker.join();

  lg(mgutils::Debug) << "Destructor RawSocketImpl\n";
}

}
}
}