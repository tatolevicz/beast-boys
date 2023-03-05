//
// Created by Arthur Motelevicz on 17/02/23.
//

#ifndef BROKERAPP_NETWORKSETTINGS_H
#define BROKERAPP_NETWORKSETTINGS_H

#include "RestApi.h"
#include <memory>
#include <openssl/hmac.h>

namespace bb{
namespace network{
    class CryptoHelper{
    public:
        
      inline static std::string hmac_sha256(const char *key, std::size_t klen, const char *data, std::size_t dlen) {
          std::uint8_t digest[EVP_MAX_MD_SIZE];
          std::uint32_t dilen{};

          auto p = ::HMAC(
                  ::EVP_sha256()
                  ,key
                  ,klen
                  ,(std::uint8_t *)data
                  ,dlen
                  ,digest
                  ,&dilen
          );
          assert(p);

          return b2a_hex(digest, dilen);
      }

      inline static std::string b2a_hex(const std::uint8_t *p, std::size_t n) {
        static const char hex[] = "0123456789abcdef";
        std::string res;
        res.reserve(n * 2);

        for ( auto end = p + n; p != end; ++p ) {
            const std::uint8_t v = (*p);
            res += hex[(v >> 4) & 0x0F];
            res += hex[v & 0x0F];
        }

        return res;
      }
    };
}
}
#endif //BROKERAPP_NETWORKSETTINGS_H
