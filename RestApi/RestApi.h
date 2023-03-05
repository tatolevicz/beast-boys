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

    NetworkFileResponse downloadFile(NetworkRequestSettings& settings,
                                     const std::string& path,
                                     const FileResponseCallback& cb = nullptr);

private:
    NetworkResponse execute(NetworkRequestSettings& settings, const RequestType type, const ResponseCallback& cb);
    NetworkFileResponse execute(NetworkRequestSettings& settings, const RequestType type, const FileResponseCallback& cb);

    std::string _port;
    std::size_t _timeout;

    void run();

    void runAsync();

    DataResponse<rapidjson::Document> validateJson(const std::string& data);
    void validateJson(const std::string& data, NetworkResponse& httpResponse);
    void validateResponse(int http_result_code, Response& response);
    void validateData(const std::string& data, DataResponse<std::string>& res);

    std::unique_ptr<boost::asio::io_context> _apictx;
    std::unique_ptr<BoostInternalImpl> _pimpl;
    std::map<std::string, std::string> _errorCodes;
};


}
}
}

#endif //BROKERAPP_RESTAPI_H
