//
// Created by Arthur Motelevicz on 04/03/23.
//

#ifndef BINANCEBEAST_RESOLVER_H
#define BINANCEBEAST_RESOLVER_H

#include "SharedState.h"

namespace bb {
namespace network {
namespace ws {

    class Resolver : public std::enable_shared_from_this<Resolver> {
    public:
        Resolver(std::shared_ptr<SharedState> state, std::shared_ptr<Stream> stream, boost::asio::io_context &ioc);
        void run();
        void onResolve(boost::system::error_code ec, const boost::asio::ip::tcp::resolver::results_type& res);
    private:
        std::shared_ptr<Stream> _stream{nullptr};
        boost::asio::ip::tcp::resolver _resolver;
        std::shared_ptr<SharedState> _sharedState{nullptr};
    };

}
}
}


#endif //BINANCEBEAST_RESOLVER_H
