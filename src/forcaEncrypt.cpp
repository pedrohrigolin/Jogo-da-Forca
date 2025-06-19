#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include <cryptopp/cryptlib.h>
#include <cryptopp/sha.h>
#include <cryptopp/md5.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/base64.h>

#include <string>

#include "forcaEncrypt.h"

namespace forcaEncrypt {

    /**
     * Codifica uma string para Base64.
     * Inspirada na função base64_encode do PHP.
     *
     * @param input String a ser codificada.
     * @return String codificada em Base64.
     */
    std::string base64_encode(const std::string& input) {
    
        // String que irá armazenar o resultado final
        std::string encoded_string;

        // Cria um "Sink" que joga os dados processados para dentro da nossa string
        CryptoPP::StringSink* sink = new CryptoPP::StringSink(encoded_string);

        // Cria um codificador Base64 que se conecta ao nosso sink
        CryptoPP::Base64Encoder* encoder = new CryptoPP::Base64Encoder(sink, false); // false = não insere quebras de linha

        // Cria uma "Fonte" com a nossa string de entrada e a conecta ao codificador
        CryptoPP::StringSource source(input, true, encoder); // true = "pump all" -> processa tudo de uma vez

        return encoded_string;

    }

    /**
     * Decodifica uma string codificada em Base64.
     * Inspirada na função base64_decode do PHP.
     *
     * @param input String codificada em Base64.
     * @return String decodificada.
     */
    std::string base64_decode(const std::string& input) {
        
        // String que irá armazenar o resultado final
        std::string decoded_string;

        // Cria um "Sink" que joga os dados processados para dentro da nossa string
        CryptoPP::StringSink* sink = new CryptoPP::StringSink(decoded_string);

        // Cria um codificador Base64 que se conecta ao nosso sink
        CryptoPP::Base64Decoder* decoder = new CryptoPP::Base64Decoder(sink); // false = não insere quebras de linha

        // Cria uma "Fonte" com a nossa string de entrada e a conecta ao codificador
        CryptoPP::StringSource source(input, true, decoder); // true = "pump all" -> processa tudo de uma vez

        return decoded_string;

    }

    /**
     * Calcula o hash SHA1 de uma string.
     * Inspirada na função sha1 do PHP.
     *
     * @param input String de entrada.
     * @return Hash SHA1 em hexadecimal (letras minúsculas).
     */
    std::string sha1(const std::string& input) {

        std::string digest; // String para armazenar o resultado hexadecimal

        // Cria o objeto do algoritmo de hash
        CryptoPP::SHA1 hash;

        // Monta o pipeline
        CryptoPP::StringSource(input, true,

            new CryptoPP::HashFilter(hash,

                new CryptoPP::HexEncoder(

                    new CryptoPP::StringSink(digest),
                    false // false = letras minúsculas para o hexadecimal

                )

            )

        );

        return digest;

    }

    /**
     * Calcula o hash MD5 de uma string.
     * Inspirada na função md5 do PHP.
     *
     * @param input String de entrada.
     * @return Hash MD5 em hexadecimal (letras minúsculas).
     */
    std::string md5(const std::string& input) {

        std::string digest;

        CryptoPP::Weak::MD5 hash;

        // O pipeline é exatamente o mesmo
        CryptoPP::StringSource(input, true,

            new CryptoPP::HashFilter(hash,

                new CryptoPP::HexEncoder(

                    new CryptoPP::StringSink(digest),
                    false // letras minúsculas

                )

            )

        );

        return digest;

    }

}