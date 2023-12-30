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

    std::ifstream file("mess.txt");
    std::string input;
    std::string line;
    while (std::getline(file, line))
    {
        input += line;
    }

    SHA1 checksum;
    checksum.update(input);
    const string hash = checksum.final();

    // print the hash( hexToBitset)  of the message to file signature.txt
    std::ofstream outFile2("signature.txt");
    if (!outFile2)
    {
        std::cerr << "Error opening output file: signature.txt" << std::endl;
        return 1;
    }
    outFile2 << hexToBitset(hash) << std::endl;
    outFile2.close();

    // generate p and q of Alice for RSA using largePrime function
    std::bitset<128> p = generateLargePrime();
    std::bitset<128> q = generateLargePrime();

    // generate p and q of Bob for RSA using largePrime function
    std::bitset<128> p2 = generateLargePrime();
    std::bitset<128> q2 = generateLargePrime();

    // generate key for Alice and Bob using p and q of Alice and Bob respectively
    vector<bitset<128>> key1 = keyGen(p, q);
    vector<bitset<128>> key2 = keyGen(p2, q2);

    // generate encrypt key and decrypt key for Alice and Bob using key of Alice and Bob respectively
    bitset<128> e_A = key1[0];
    bitset<128> d_A = key1[1];
    bitset<128> e_B = key2[0];
    bitset<128> d_B = key2[1];

    // encrypt key with RSA and print it to file encrypted_key.txt
    std::ofstream outFile3("encrypted_key.txt");
    if (!outFile3)
    {
        std::cerr << "Error opening output file: encrypted_key.txt" << std::endl;
        return 1;
    }
    std::bitset<128> encrypted_key = encrypt(e_B, key2[2], key);
    outFile3 << encrypted_key << std::endl;
    outFile3.close();

    // encrypt signature.txt with RSA and print it to file encrypted_signature.txt
    std::ofstream outFile6("encrypted_signature.txt");
    if (!outFile6)
    {
        std::cerr << "Error opening output file: encrypted_signature.txt" << std::endl;
        return 1;
    }
    std::bitset<128> encrypted_s = encrypt(d_A, key1[2], hexToBitset(hash));
    outFile6 << encrypted_s << std::endl;
    outFile6.close();

    // decrypt key with RSA and print it to file decrypted_key.txt
    std::ofstream outFile4("decrypted_key.txt");
    if (!outFile4)
    {
        std::cerr << "Error opening output file: decrypted_key.txt" << std::endl;
        return 1;
    }
    std::bitset<128> decrypted_key = decrypt(d_B, key1[2], encrypted_key);
    outFile4 << decrypted_key << std::endl;
    outFile4.close();

    // decrypt the cipher text blocks with AES and print the plain text blocks to file decrypted_aes.txt
    std::ofstream outFile5("decrypted_aes.txt");
    if (!outFile5)
    {
        std::cerr << "Error opening output file: decrypted_aes.txt" << std::endl;
        return 1;
    }
    for (const auto &block : message_blocks)
    {
        std::bitset<128> decrypted_block = decryptBlock(decrypted_key, block);
        outFile5 << decrypted_block << std::endl;
    }
    outFile5.close();

    // decrypt signature.txt with RSA and print it to file decrypted_signature.txt
    std::ofstream outFile7("decrypted_signature.txt");
    if (!outFile7)
    {
        std::cerr << "Error opening output file: decrypted_signature.txt" << std::endl;
        return 1;
    }
    std::bitset<128> decrypted_s = decrypt(e_A, key2[2], encrypted_s);
    outFile7 << decrypted_s << std::endl;

    // compare the hash of the message with the decrypted signature
    if (decrypted_s == hexToBitset(hash))
    {
        std::cout << "The message is authentic." << std::endl;
    }
    else
    {
        std::cout << "The message is not authentic." << std::endl;
    }
    outFile7.close();

    int result = system("py cloud.py");
    if (result == 0)
    {
        std::cout << "Script executed successfully." << std::endl;
    }
    else
    {
        std::cerr << "Script execution failed." << std::endl;
    }
    return 0;
}
