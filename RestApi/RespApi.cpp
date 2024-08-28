//
// Created by Arthur Motelevicz on 01/03/23.
//

#include "RestApi.h"
#include "BoostInternalImpl.h"
#include <boost/asio.hpp>
#include "Logger.h"

#ifdef __APPLE__
  #include <pthread.h>
#endif

namespace bb::network::rest
{
  RestApi::RestApi(std::string port, TaskExecutionType executionType, std::size_t timeout):
  _port(port),
  _timeout(timeout),
  _pimpl(std::make_unique<BoostInternalImpl>(_ioc, executionType, std::move(port), timeout)),
  _executionType(executionType)
  {
    if(_executionType == TaskExecutionType::BB_ASYNCH)
      startAsyncContext();
  }

  RestApi::~RestApi()
  {
    if(_executionType == TaskExecutionType::BB_ASYNCH)
    {
      _destructorCalled = true;
      _ioc.stop();
      _work.reset();
      if (_worker.joinable())
        _worker.join();
    }

    lg(mgutils::Debug) << "Destructor RestApi\n";
  }

  void RestApi::startAsyncContext()
  {
    _work = std::make_shared<boost::asio::io_context::work>(_ioc);
    _worker = std::thread([&]()
    {
  #ifdef __APPLE__
      pthread_setname_np("BeastBoys-RestApi-Worker");
  #endif

      while(!_destructorCalled)
      {
        try
        {
          _ioc.run();
        }
        catch (const boost::system::system_error &e)
        {
          REPORT_ASIO_ERROR_(e.code())
          restartAsyncContext();
        }
      }
    });
  }


  void RestApi::restartAsyncContext()
  {
    _work.reset();
    _ioc.restart();
    startAsyncContext();
  }

  NetworkResponse RestApi::post(NetworkRequestSettings &settings, const ResponseCallback &cb)
  {
    return execute(settings, RequestType::post, cb);
  }

  NetworkResponse RestApi::get(NetworkRequestSettings &settings, const ResponseCallback &cb)
  {
    return execute(settings, RequestType::get, cb);
  }

  NetworkResponse RestApi::put(NetworkRequestSettings &settings, const ResponseCallback &cb)
  {
    return execute(settings, RequestType::put, cb);
  }

  NetworkResponse RestApi::del(NetworkRequestSettings &settings, const ResponseCallback &cb) {
      return execute(settings, RequestType::delete_, cb);
  }


  NetworkResponse
  RestApi::downloadFile(NetworkRequestSettings &settings,
                        const std::string &outputFilePath,
                        const ResponseCallback &cb)
  {
    return execute(settings,
                   RequestType::get,
    [cb, outputFilePath](const NetworkResponse &res)
    {
      if (res.isOk())
      {
        std::ofstream fp(outputFilePath, std::ios::binary);
        fp << res.data;
        fp.close();
      }

      if (cb)
        cb(res);
    });
  }

  NetworkResponse
  RestApi::execute(NetworkRequestSettings &settings,
                   const RequestType& type,
                   const ResponseCallback &outCb)
  {
    assert(_pimpl);
    bool async =  _executionType == TaskExecutionType::BB_ASYNCH;

    PostCallback cb =[&, outCb](  const char *fl,
                                  int ec,
                                  const std::string& errmsg,
                                  auto data,
                                  int http_result_code) -> bool
    {
      NetworkResponse response(errmsg, http_result_code);

      if (ec != 0)
      {
        lg(mgutils::Error) << fl << " - Error message: " << errmsg;
      } else
      {
        validateResponse(http_result_code, response);
        response.data = std::move(data);
      }

      if (outCb)
        outCb(response);

      return ec == 0;
    };

    NetworkResponse response = _pimpl->post(settings, type, std::move(cb));

    if (async)
      return response;


    validateResponse(response.http_result_code, response);

    if (outCb)
      outCb(response);


    return response;
  }

  void RestApi::validateResponse(int http_result_code, NetworkResponse &response)
  {
      switch (http_result_code) {
          case (int) boost::beast::http::status::ok:
          case (int) boost::beast::http::status::created:
          case (int) boost::beast::http::status::accepted:
          case (int) boost::beast::http::status::no_content:
          case (int) boost::beast::http::status::reset_content:
          case (int) boost::beast::http::status::partial_content:
          {
              response.http_result_code = http_result_code;
              response.message = "Http success!";
              break;
          }
          default: {
              response.http_result_code = http_result_code;
              response.message = "Http response error!";
          }
      }
  }

}