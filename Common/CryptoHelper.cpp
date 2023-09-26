//
// Created by Opala Studios on 26/09/23.
//
#include "CryptoHelper.h"
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

namespace bb::network {
std::string CryptoHelper::hmac_sha256(const char *key, std::size_t klen, const char *data, std::size_t dlen) {
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

std::string CryptoHelper::b2a_hex(const std::uint8_t *p, std::size_t n) {
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

std::string CryptoHelper::encrypt_aes_cbc(const std::string &plaintext, const std::string& private_key) {
    const int size = static_cast<int>(private_key.size());
    unsigned char key_hashed[size]; // SHA-256 output size
    SHA256(reinterpret_cast<const unsigned char *>(private_key.c_str()), private_key.size(), key_hashed);

    unsigned char iv[AES_BLOCK_SIZE];
    RAND_bytes(iv, AES_BLOCK_SIZE); // Gera um IV aleatório

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key_hashed, iv);

    std::vector<unsigned char> ciphertext(plaintext.size() + AES_BLOCK_SIZE);
    int len;
    int ciphertext_len;

    EVP_EncryptUpdate(ctx, &ciphertext[0], &len, reinterpret_cast<const unsigned char *>(plaintext.c_str()), plaintext.size());
    ciphertext_len = len;

    EVP_EncryptFinal_ex(ctx, &ciphertext[len], &len);
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    std::string result(reinterpret_cast<char *>(iv), AES_BLOCK_SIZE); // Começa com o IV
    result += std::string(ciphertext.begin(), ciphertext.begin() + ciphertext_len); // Seguido pelo ciphertext

    return b2a_hex(reinterpret_cast<const unsigned char *>(result.c_str()), result.size()); // Retorna em formato hexadecimal
}

std::string CryptoHelper::decrypt_aes_cbc(const std::string &hex_ciphertext, const std::string& private_key) {
    const int size = static_cast<int>(private_key.size());
    unsigned char key_hashed[size]; // SHA-256 output size
    SHA256(reinterpret_cast<const unsigned char *>(private_key.c_str()), private_key.size(), key_hashed);

    std::string ciphertext = a2b_hex(hex_ciphertext);
    unsigned char iv[AES_BLOCK_SIZE];
    std::copy(ciphertext.begin(), ciphertext.begin() + AES_BLOCK_SIZE, iv); // Extrai o IV

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key_hashed, iv);

    std::vector<unsigned char> plaintext(ciphertext.size());
    int len;
    int plaintext_len;

    EVP_DecryptUpdate(ctx, &plaintext[0], &len, reinterpret_cast<const unsigned char *>(ciphertext.c_str()) + AES_BLOCK_SIZE, ciphertext.size() - AES_BLOCK_SIZE);
    plaintext_len = len;

    EVP_DecryptFinal_ex(ctx, &plaintext[len], &len);
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return std::string(plaintext.begin(), plaintext.begin() + plaintext_len);
}

std::string CryptoHelper::a2b_hex(const std::string &hex) {
    std::string bytes;
    for (unsigned int i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        char byte = (char) strtol(byteString.c_str(), nullptr, 16);
        bytes += byte;
    }
    return bytes;
}

}
