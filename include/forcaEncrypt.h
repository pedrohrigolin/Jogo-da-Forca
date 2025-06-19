#ifndef FORCA_ENCRYPT_H
#define FORCA_ENCRYPT_H

#include <string>

namespace forcaEncrypt {

    std::string base64_encode(const std::string& input);

    std::string base64_decode(const std::string& input);

    std::string sha1(const std::string& input);

    std::string md5(const std::string& input);

}

#endif