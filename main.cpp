#include "Header.h"
#include "cloud.cpp"
#include "RSA_Encryption_Decryption.cpp"
#include "aes.cpp"
#include "sha1.cpp"

int main()
{
    // generate original key using random number generator bitset<128> binary
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    std::bitset<128> original_key;
    for (int i = 0; i < 16; ++i)
    {
        original_key[i] = dis(gen);
    }
    std::cout << "Original key: " << original_key << std::endl;

    // Sinh khóa k
    std::bitset<128> generated_key = generateKey(original_key);

    // đọc file mess.txt và lưu vào message
    std::ifstream inFile("mess.txt");
    if (!inFile)
    {
        std::cerr << "Error opening input file: mess.txt" << std::endl;
        return 1;
    }
    std::string message;
    std::getline(inFile, message);
    inFile.close();

    // Chia tin nhắn thành các khối 128 bit
    std::vector<std::bitset<128>> message_blocks = divideMessageIntoBlocks(message);

    // Mã hóa các khối tin nhắn và in các khối văn bản mã hóa ra file encrypted_aes.txt
    std::ofstream outFile("encrypted_aes.txt");
    if (!outFile)
    {
        std::cerr << "Error opening output file: encrypted_aes.txt" << std::endl;
        return 1;
    }

    for (const auto &block : message_blocks)
    {
        std::bitset<128> encrypted_block = encryptBlock(generated_key, block);
        outFile << encrypted_block << std::endl;
    }
    outFile.close();

    // Đọc file encrypted_aes.txt, giải mã các khối và in ra màn hình
    std::ifstream inFile2("encrypted_aes.txt");
    if (!inFile2)
    {
        std::cerr << "Error opening input file: encrypted_aes.txt" << std::endl;
        return 1;
    }

    std::vector<std::bitset<128>> encrypted_blocks;
    std::string line;
    while (std::getline(inFile2, line))
    {
        encrypted_blocks.push_back(std::bitset<128>(line));
    }
    inFile2.close();

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
    std::bitset<128> encrypted_key = encrypt(e_B, key2[2], generated_key);
    outFile3 << encrypted_key << std::endl;
    outFile3.close();

    // // encrypt signature.txt with RSA and print it to file encrypted_signature.txt
    // std::ofstream outFile6("encrypted_signature.txt");
    // if (!outFile6)
    // {
    //     std::cerr << "Error opening output file: encrypted_signature.txt" << std::endl;
    //     return 1;
    // }
    // std::bitset<128> encrypted_s = encrypt(d_A, key1[2], hexToBitset(hash));
    // outFile6 << encrypted_s << std::endl;
    // outFile6.close();

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

    // // decrypt signature.txt with RSA and print it to file decrypted_signature.txt
    // std::ofstream outFile7("decrypted_signature.txt");
    // if (!outFile7)
    // {
    //     std::cerr << "Error opening output file: decrypted_signature.txt" << std::endl;
    //     return 1;
    // }
    // std::bitset<128> decrypted_s = decrypt(e_A, key2[2], encrypted_s);
    // outFile7 << decrypted_s << std::endl;

    // // compare the hash of the message with the decrypted signature
    // if (decrypted_s == hexToBitset(hash))
    // {
    //     std::cout << "The message is authentic." << std::endl;
    // }
    // else
    // {
    //     std::cout << "The message is not authentic." << std::endl;
    // }
    // outFile7.close();

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
