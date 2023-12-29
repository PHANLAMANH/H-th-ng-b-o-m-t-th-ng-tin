#include "test.cpp"
#include <openssl/sha.h>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <iostream>
#include <string.h>
using namespace std;
int main()
{
    string s = "hello";
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, s.c_str(), s.size());
    SHA256_Final(hash, &sha256);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        printf("%02x", hash[i]);
    }
    printf("\n");
    return 0;
}