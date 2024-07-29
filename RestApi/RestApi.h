//
// Created by Arthur Motelevicz on 16/02/23.
//

#ifndef BEASTBOYS_RESTAPI_H
#define BEASTBOYS_RESTAPI_H

#include <string>
#include "NetworkTypes.h"
#include "BoostInternalImpl.h"

namespace bb::network::rest
{
class NetworkRequestSettings;

class RestApi {
public:
    explicit RestApi(std::string port,
                     TaskExecutionType executionType = TaskExecutionType::BB_ASYNCH,
                     std::size_t timeout = 10000);

    virtual ~RestApi();

    std::size_t getTimeout() const
    {
        return _timeout;
    }

    NetworkResponse post(NetworkRequestSettings& settings,
                         const ResponseCallback& cb = nullptr) ;

    NetworkResponse get(NetworkRequestSettings& settings,
                        const ResponseCallback& cb = nullptr) ;

    NetworkResponse put(NetworkRequestSettings& settings,
                        const ResponseCallback& cb = nullptr) ;

    NetworkResponse del(NetworkRequestSettings& settings,
                        const ResponseCallback& cb = nullptr) ;

    NetworkResponse downloadFile(NetworkRequestSettings& settings,
                                 const std::string& path,
                                 const ResponseCallback& cb);

private:
  void startAsyncContext();
  void restartAsyncContext();

  NetworkResponse execute(NetworkRequestSettings& settings,
                          const RequestType& type,
                          const ResponseCallback& cb);
  std::string _port;
  std::size_t _timeout;

  void validateResponse(int http_result_code,
                        NetworkResponse& response);

  std::thread _worker;
  std::shared_ptr<boost::asio::io_context::work> _work{nullptr};
  boost::asio::io_context _ioc;
  std::unique_ptr<BoostInternalImpl> _pimpl;
  bool _destructorCalled = false;
  TaskExecutionType _executionType;
};

}

#endif //BEASTBOYS_RESTAPI_H
