#include <sha.h>
#include <filters.h>
#include <hex.h>
#include <iostream>

int main()
{
    std::string data = "Hello, World!";

    CryptoPP::SHA256 hash;
    std::string digest;

    CryptoPP::StringSource ss(data, true,
                              new CryptoPP::HashFilter(hash,
                                                       new CryptoPP::HexEncoder(
                                                           new CryptoPP::StringSink(digest)) // HexEncoder
                                                       )                                     // HashFilter
    );                                                                                       // StringSource

    std::cout << "Digest: " << digest << std::endl;

    return 0;
}
