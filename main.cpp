#include "Header.h"
#include "cloud.cpp"
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

    // read the message from the file and store it with string type
    std::string message;
    std::getline(inFile, message);
    inFile.close();

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
    // Divide the message into 128-bit blocks
    std::vector<std::bitset<128>> message_blocks = divideMessageIntoBlocks(message);

    // encrypt the message blocks and print the cipher text blocks to file encrypted_aes.txt
    std::ofstream outFile("encrypted_aes.txt");
    if (!outFile)
    {
        std::cerr << "Error opening output file: encrypted_aes.txt" << std::endl;
        return 1;
    }

    for (const auto &block : message_blocks)
    {
        std::bitset<128> encrypted_block = encryptBlock(key, block);
        outFile << encrypted_block << std::endl;
    }
    outFile.close();

    // generate the hash of the message
    SHA1 checksum;
    checksum.update(message);
    const std::string hash = checksum.final();
    // print the hash to file signature.txt
    std::ofstream outFile2("signature.txt");
    if (!outFile2)
    {
        std::cerr << "Error opening output file: signature.txt" << std::endl;
        return 1;
    }
    outFile2 << hash << std::endl;
    outFile2.close();
}
