#include "Header.h"
#include "structures.h"
#include "modulo.cpp"
#include "cloud.cpp"
#include "RSA_Encryption_Decryption.cpp"
#include "RSA_Encryption_Decryption.cpp"
#include "test.cpp"
#include "sha1.cpp"

int main()
{
    // input file stream message.txt to read the message from the file
    std::ifstream inFile("mess.txt");
    if (!inFile)
    {
        std::cerr << "Error opening input file" << std::endl;
        return 1;
    }

    // generate original key ( aes key )
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    std::bitset<128> original_key;
    for (int i = 0; i < 16; ++i)
    {
        original_key[i] = dis(gen);
    }

    // generate key from original key
    std::bitset<128> key = generateKey(original_key);
    std::cout << "key: " << key << std::endl;
    return 0;
}
