#include "Header.h"
#include "SHA1.h"
#include "AES.h"
#include "RSA.h"

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

    // hash message and print to file signature.txt
    SHA1 checksum;
    checksum.update(message);
    const string hash = checksum.final();
    std::bitset<128> hash_bitset = hexToBin(hash);
    std::string hash_bitset_string = hash_bitset.to_string();
    std::ofstream outFile8("signature.txt");
    if (!outFile8)
    {
        std::cerr << "Error opening output file: signature.txt" << std::endl;
        return 1;
    }
    outFile8 << hash_bitset_string;
    outFile8.close();

    // chia message thành các block 128 bit
    std::vector<std::bitset<128>> blocks = divideMessageIntoBlocks(message);

    // Mã hóa các khối tin nhắn và in các khối văn bản mã hóa ra file cipher.txt
    std::ofstream outFile("cipher.txt");
    if (!outFile)
    {
        std::cerr << "Error opening output file: cipher.txt" << std::endl;
        return 1;
    }
    for (int i = 0; i < blocks.size(); ++i)
    {
        std::bitset<128> encrypted_block = encryptBlock(blocks[i], generated_key);
        outFile << encrypted_block;
    }
    outFile.close();

    // get Alice encrypt key and n from file Alice_Encrypt_key.txt, first line is key eA, second line is n , bitset<128> eA, n
    std::ifstream inFile1("Alice_Encrypt_key.txt");
    if (!inFile1)
    {
        std::cerr << "Error opening input file: Alice_Encrypt_key.txt" << std::endl;
        return 1;
    }
    std::string eA, nA;
    std::getline(inFile1, eA);
    std::getline(inFile1, nA);

    // convert string to bitset
    std::bitset<128> eA_bitset;
    std::bitset<128> nA_bitset;
    for (int i = 0; i < 128; ++i)
    {
        eA_bitset[i] = eA[i] - '0';
        nA_bitset[i] = nA[i] - '0';
    }
    inFile1.close();

    // get Bob encrypt key and n from file Bob_Encrypt_key.txt, first line is key eB, second line is n , bitset<128> eB, n
    std::ifstream inFile2("Bob_Encrypt_key.txt");
    if (!inFile2)
    {
        std::cerr << "Error opening input file: Bob_Encrypt_key.txt" << std::endl;
        return 1;
    }
    std::string eB, nB;
    std::getline(inFile2, eB);
    std::getline(inFile2, nB);

    // convert string to bitset
    std::bitset<128> eB_bitset;
    std::bitset<128> nB_bitset;
    for (int i = 0; i < 128; ++i)
    {
        eB_bitset[i] = eB[i] - '0';
        nB_bitset[i] = nB[i] - '0';
    }
    inFile2.close();

    // encrypt generated key using RSA with Bob encrypt key and n
    std::bitset<128> encrypted_key = encrypt_RSA(generated_key, eB_bitset, nB_bitset);

    // print encrypted key to file encrypted_rsa_key.txt
    std::ofstream outFile1("encrypted_rsa_key.txt");
    if (!outFile1)
    {
        std::cerr << "Error opening output file: encrypted_rsa_key.txt" << std::endl;
        return 1;
    }
    outFile1 << encrypted_key;
    outFile1.close();

    // decrypt encrypted key using RSA with Bob nB_bitset
    std::bitset<128> decrypted_key = decrypt_RSA(encrypted_key, eB_bitset, nB_bitset);

    // print decrypted key to file decrypted_rsa_key.txt
    std::ofstream outFile2("decrypted_rsa_key.txt");
    if (!outFile2)
    {
        std::cerr << "Error opening output file: decrypted_rsa_key.txt" << std::endl;
        return 1;
    }
    outFile2 << decrypted_key;
    outFile2.close();

    // use decrypted_key to decrypt cipher.txt using decryptBlock, Chuyển các khối đã mã hóa thành tin nhắn ban đầu bằng blocksToMessage and print to file decrypted_cipher.txt
    std::ifstream inFile4("cipher.txt");
    if (!inFile4)
    {
        std::cerr << "Error opening input file: cipher.txt" << std::endl;
        return 1;
    }
    std::string cipher;
    std::getline(inFile4, cipher);
    inFile4.close();
    std::vector<std::bitset<128>> cipher_blocks = divideMessageIntoBlocks(cipher);
    std::ofstream outFile4("decrypted_cipher.txt");
    if (!outFile4)
    {
        std::cerr << "Error opening output file: decrypted_cipher.txt" << std::endl;
        return 1;
    }
    for (int i = 0; i < cipher_blocks.size(); ++i)
    {
        std::bitset<128> decrypted_block = decryptBlock(cipher_blocks[i], decrypted_key);
        // sử dụng blocksToMessage để chuyển các khối đã giải mã thành tin nhắn ban đầu
        std::vector<std::bitset<128ULL>> decrypted_blocks;
        decrypted_blocks.push_back(decrypted_block);
        std::string decrypted_message = blocksToMessage(decrypted_blocks, message.length());
        outFile4 << decrypted_message;
    }
    outFile4.close();

    // hash decrypted_cipher.txt and print to file verification.txt
    std::ifstream inFile5("decrypted_cipher.txt");
    if (!inFile5)
    {
        std::cerr << "Error opening input file: decrypted_cipher.txt" << std::endl;
        return 1;
    }
    std::string decrypted_cipher;
    std::getline(inFile5, decrypted_cipher);
    inFile5.close();
    SHA1 checksum1;
    checksum1.update(decrypted_cipher);
    const string hash1 = checksum1.final();
    std::bitset<128> hash_bitset1 = hexToBin(hash1);
    std::string hash_bitset_string1 = hash_bitset1.to_string();
    std::ofstream outFile5("verification.txt");
    if (!outFile5)
    {
        std::cerr << "Error opening output file: verification.txt" << std::endl;
        return 1;
    }
    outFile5 << hash_bitset_string1;
    outFile5.close();
    // compare hash_bitset_string and hash_bitset_string1 to verify the message
    if (hash_bitset_string == hash_bitset_string1)
    {
        std::cout << "Message is verified" << std::endl;
    }
    else
    {
        std::cout << "Message is not verified" << std::endl;
    }

    //     int result = system("py cloud.py");
    // if (result == 0)
    // {
    //     std::cout << "Script executed successfully." << std::endl;
    // }
    // else
    // {
    //     std::cerr << "Script execution failed." << std::endl;
    // }
    return 0;
}
