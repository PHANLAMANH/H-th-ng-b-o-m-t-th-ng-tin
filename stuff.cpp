#include "RSA_Encryption_Decryption.cpp"
#include "test.cpp"

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

    // In ra khóa đã sinh
    std::cout << "Generated key K: " << generated_key << std::endl;

    std::cout << "original key + generated key: " << addBin(original_key, generated_key) << std::endl;

    // std::string message = "this is a test, my name is Lam Anh. "; // Tin nhắn cần chia thành các khối

    // // Chia tin nhắn thành các khối 128 bit
    // std::vector<std::bitset<128>> message_blocks = divideMessageIntoBlocks(message);

    // // In ra các khối tin nhắn 128 bit
    // for (const auto &block : message_blocks)
    // {
    //     std::cout << "Block: " << block << std::endl;
    // }
    // for (const auto &block : message_blocks)
    // {
    //     std::bitset<128> encrypted_block = encryptBlock(generated_key, block);
    //     std::cout << "Encrypted Block: " << encrypted_block << std::endl;
    // }
    // for (const auto &block : message_blocks)
    // {
    //     std::bitset<128> decrypted_block = decryptBlock(generated_key, block);
    //     std::cout << "Decrypted Block: " << decrypted_block << std::endl;
    // }
    // // Chuyển các khối đã giải mã thành tin nhắn ban đầu
    // std::string decrypted_message = blocksToMessage(message_blocks, message.length());
    // std::cout << "Decrypted Message: " << decrypted_message << std::endl;

    return 0;
}