//
// Created by Arthur Motelevicz on 17/02/23.
//

#ifndef BROKERAPP_NETWORKSETTINGS_H
#define BROKERAPP_NETWORKSETTINGS_H

#include "RestApi.h"
#include <memory>


namespace bb{
namespace network{
    class CryptoHelper{
    public:
        static std::string hmac_sha256(const char *key, std::size_t klen, const char *data, std::size_t dlen);

        static std::string b2a_hex(const std::uint8_t *p, std::size_t n);

        static std::string encrypt_aes_cbc(const std::string &plaintext, const std::string& private_key);

        static std::string decrypt_aes_cbc(const std::string &hex_ciphertext, const std::string& private_key);

        static std::string a2b_hex(const std::string &hex);
    };
}
}
#endif //BROKERAPP_NETWORKSETTINGS_H
