//
// Created by Arthur Motelevicz on 01/03/23.
//

#include "RestApi.h"
#include "BoostInternalImpl.h"
#include <boost/asio.hpp>
#include "Logger.h"
#include <pthread.h>

namespace bb {
namespace network {
namespace rest {

RestApi::RestApi(std::string port, TaskExecutionType executionType, std::size_t timeout):
_port(port),
_timeout(timeout),
_pimpl(std::make_unique<BoostInternalImpl>(_ioc, executionType, std::move(port), timeout)),
_executionType(executionType)
{
//    _errorCodes.insert_or_assign("statusCode", "message");
//    _errorCodes.insert_or_assign("code", "msg");

    if(_executionType == TaskExecutionType::ASYNCH)
        startAsyncContext();
}

RestApi::~RestApi(){
    if(_executionType == TaskExecutionType::ASYNCH) {
        _destructorCalled = true;
//        _ioc.stop();
        _work.reset();
        if (_worker.joinable())
            _worker.join();
    }

    std::cout << "Destructor RestApi\n";
}

void RestApi::startAsyncContext(){
    _work = std::make_shared<boost::asio::io_context::work>(_ioc);
    _worker = std::thread([&]() {
        pthread_setname_np("Binance-beast-RestApi-Worker");
        while(!_destructorCalled) {
            try {
                _ioc.run();
            }
            catch (const boost::system::system_error &e) {
                REPORT_ASIO_ERROR_(e.code())
                restartAsyncContext();
            }
        }
    });
}


void RestApi::restartAsyncContext(){
    _work.reset();
    _ioc.restart();
    startAsyncContext();
}
NetworkResponse RestApi::post(NetworkRequestSettings &settings, const ResponseCallback &cb) {
    return execute(settings, RequestType::post, cb);
}

NetworkResponse RestApi::get(NetworkRequestSettings &settings, const ResponseCallback &cb) {
    return execute(settings, RequestType::get, cb);
}

NetworkResponse RestApi::put(NetworkRequestSettings &settings, const ResponseCallback &cb) {
    return execute(settings, RequestType::put, cb);
}

NetworkResponse RestApi::del(NetworkRequestSettings &settings, const ResponseCallback &cb) {
    return execute(settings, RequestType::delete_, cb);
}


NetworkResponse RestApi::downloadFile(NetworkRequestSettings &settings,
                                          const std::string &outputFilePath,
                                          const ResponseCallback &cb) {
    return execute(settings, RequestType::get, [cb, outputFilePath](const NetworkResponse &res) {
        if (res.isOk()) {
            std::ofstream fp(outputFilePath);
            fp << res.data;
            fp.close();
        }

        if (cb)
            cb(res);
    });
}

NetworkResponse
RestApi::execute(NetworkRequestSettings &settings, const RequestType type, const ResponseCallback &outCb) {
    assert(_pimpl);
    bool async =  _executionType == TaskExecutionType::ASYNCH;

    PostCallback cb = [&, outCb](
            const char *fl,
            int ec, std::string errmsg,
            auto data,
            int http_result_code
        ) -> bool {

        NetworkResponse response(errmsg, http_result_code);

        if (ec != 0) {
            std::cerr << fl << " - Error message: " << errmsg << "\n";
        } else {
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

void RestApi::run() {
//    _ioc.restart();
//    _ioc.run();
}

//void RestApi::runAsync() {
//    std::thread worker(&RestApi::run, this);
//    worker.detach();
//}

void RestApi::validateResponse(int http_result_code, NetworkResponse &response) {
    switch (http_result_code) {
        case (int) boost::beast::http::status::ok:
        case (int) boost::beast::http::status::created:
        case (int) boost::beast::http::status::accepted:
        case (int) boost::beast::http::status::no_content:
        case (int) boost::beast::http::status::reset_content:
        case (int) boost::beast::http::status::partial_content: {
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

//void RestApi::validateJson(const std::string &data, NetworkResponse &res) {
//    auto document = Json::document();
//    if (!Json::parse(data, document)) {
//        res.code = document.GetParseError();
//        res.message = "Error when running the process";
//    } else if (document.IsObject()) {
//        for (auto item: _errorCodes) {
//            if (_pimpl->getResponseHelper()->isApiError(document, item.first, item.second)) {
//                auto error = _pimpl->getResponseHelper()->constructError(document, item.first, item.second);
//                res.code = error.first;
//                res.message = error.second;
//                res.reply.clear();
//            }
//        }
//    }
//
//    res.data = std::move(document);
//}

//DataResponse<rapidjson::Document> RestApi::validateJson(const std::string &data) {
//    DataResponse<rapidjson::Document> res;
//
//    auto document = Json::document();
//    if (!Json::parse(data, document) && !res.isHttpOk()) {
//        res.code = document.GetParseError();
//        res.message = "Error when running the process";
//        std::cerr << __MAKE_FILELINE << " - Parse error\n";
//        return res;
//    }
//
//    if (document.IsObject()) {
//        for (auto item: _errorCodes) {
//            if (_pimpl->getResponseHelper()->isApiError(document, item.first, item.second)) {
//                auto error = _pimpl->getResponseHelper()->constructError(document, item.first, item.second);
//                res.code = error.first;
//                res.message = error.second;
//                res.reply.clear();
//
//                return res;
//            }
//        }
//    }
//
//    res.data = std::move(document);
//
//    return res;
//}

//void RestApi::validateData(const std::string &data, DataResponse<std::string> &response) {
////    DataResponse<std::string> response;
//
//    if (_pimpl->getResponseHelper()->isXML(data)) {
//        response.code = -1;
//        response.message = data;
//        __MAKE_ERRMSG(response, std::string(" Error message: Download error. Check the error in the data"));
//        response.reply.clear();
//    } else {
//        auto res = validateJson(data);
//        if (res.code != rapidjson::ParseErrorCode::kParseErrorDocumentEmpty &&
//            res.code != rapidjson::ParseErrorCode::kParseErrorValueInvalid) {
//            response.code = res.code;
//            response.message = res.message;
//        }
//    }
//
//    response.data = data;
//}

}
}
}