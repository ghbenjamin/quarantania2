#pragma once
#include <string>

// https://renenyffenegger.ch/notes/development/Base64/Encoding-and-decoding-base-64-with-cpp/
// Altered by brh on 13/07/2020

std::string base64_encode     (std::string const& s, bool url = false);
std::string base64_encode_pem (std::string const& s);
std::string base64_encode_mime(std::string const& s);

std::string base64_decode(std::string const& s, bool remove_linebreaks = false);
std::string base64_encode(unsigned char const*, size_t len, bool url = false);
