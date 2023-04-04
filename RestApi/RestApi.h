//
// Created by Arthur Motelevicz on 16/02/23.
//

#ifndef BROKERAPP_RESTAPI_H
#define BROKERAPP_RESTAPI_H

#include <string>
#include "NetworkTypes.h"
#include "BoostInternalImpl.h"

namespace bb {
namespace network {
namespace rest {

class NetworkRequestSettings;

class RestApi {
public:
    explicit RestApi(std::string port, std::size_t timeout = 10000);
    virtual ~RestApi() = default;

    std::size_t getTimeout() const {
        return _timeout;
    }

    NetworkResponse post(NetworkRequestSettings& settings, const ResponseCallback& cb = nullptr) ;

    NetworkResponse get(NetworkRequestSettings& settings, const ResponseCallback& cb = nullptr) ;

    NetworkResponse put(NetworkRequestSettings& settings, const ResponseCallback& cb = nullptr) ;

    NetworkResponse del(NetworkRequestSettings& settings, const ResponseCallback& cb = nullptr) ;

    NetworkResponse downloadFile(NetworkRequestSettings& settings,
                                 const std::string& path,
                                 const ResponseCallback& cb);

private:
    NetworkResponse execute(NetworkRequestSettings& settings, const RequestType type, const ResponseCallback& cb);

    std::string _port;
    std::size_t _timeout;

    void run();

    void runAsync();

    void validateResponse(int http_result_code, NetworkResponse& response);

    std::unique_ptr<boost::asio::io_context> _apictx;
    std::unique_ptr<BoostInternalImpl> _pimpl;
    bool runnedOnce = false;
};


}
}
}

#endif //BROKERAPP_RESTAPI_H
