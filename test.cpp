#include <iostream>
#include <random>
#include <bitset>
#include <iomanip>
#include <cstdint>
// S-box và Rcon đã được cung cấp
const uint8_t sbox[256] = {
    // 0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
    0b01100011, 0b01111100, 0b01110111, 0b01111011, 0b11110010, 0b01101011, 0b01101111, 0b11000101, // 0
    0b00110000, 0b00000001, 0b01100111, 0b00101011, 0b11111110, 0b11010111, 0b10101011, 0b01110110, // 1
    0b11001010, 0b10000010, 0b11001001, 0b01111101, 0b11111010, 0b01011001, 0b01000111, 0b11110000, // 2
    0b10101101, 0b11010100, 0b10100010, 0b10101111, 0b10011100, 0b10100100, 0b01110010, 0b11000000, // 3
    0b10110111, 0b11111101, 0b10010011, 0b00100110, 0b00110110, 0b00111111, 0b11110111, 0b11001100, // 4
    0b00110100, 0b10100101, 0b11100101, 0b11110001, 0b01110001, 0b11011000, 0b00110001, 0b00010101, // 5
    0b00000100, 0b11000111, 0b00100011, 0b11000011, 0b00011000, 0b10010110, 0b00000101, 0b10011010, // 6
    0b00000111, 0b00010010, 0b10000000, 0b11100010, 0b11101011, 0b00100111, 0b10110010, 0b01110101, // 7
    0b00001001, 0b10000011, 0b00101100, 0b00011010, 0b00011011, 0b01101110, 0b01011010, 0b10100000, // 8
    0b01010010, 0b00111011, 0b11010110, 0b10110011, 0b00101001, 0b11100011, 0b00101111, 0b10000100, // 9
    0b01010011, 0b11010001, 0b00000000, 0b11101101, 0b00100000, 0b11111100, 0b10110001, 0b01011011, // A
    0b01101010, 0b11001011, 0b10111110, 0b00111001, 0b01001010, 0b01001100, 0b01011000, 0b11001111, // B
    0b11010000, 0b11101111, 0b10101010, 0b11111011, 0b01000011, 0b01001101, 0b00110011, 0b10000101, // C
    0b01000101, 0b11111001, 0b00000010, 0b01111111, 0b01010000, 0b00111100, 0b10011111, 0b10101000, // D
    0b01010001, 0b10100011, 0b01000000, 0b10001111, 0b10010010, 0b10011101, 0b00111000, 0b11110101, // E
    0b10111100, 0b10110110, 0b11011010, 0b00100001, 0b00010000, 0b11111111, 0b11110011, 0b11010010  // F
};

const uint8_t rcon[256] = {
    // 0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
    0b10001101, 0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000, // 0
    0b10000000, 0b00011011, 0b00110110, 0b01101100, 0b11011000, 0b10101011, 0b01001101, 0b10011010, // 1
    0b00101111, 0b01011110, 0b10111100, 0b1100011, 0b11011000, 0b10010111, 0b01110101, 0b1101010,   // 2
    0b11010011, 0b10110011, 0b01111101, 0b11111010, 0b11101111, 0b11000101, 0b10010001, 0b00111001, // 3
    0b01110010, 0b11100100, 0b11010011, 0b10111101, 0b01100001, 0b11000010, 0b10011111, 0b00100101, // 4
    0b01001010, 0b10010100, 0b00110011, 0b01100110, 0b11001100, 0b10000011, 0b00011101, 0b00111010, // 5
    0b01110100, 0b11101000, 0b11001011, 0b10001101, 0b00000001, 0b00000010, 0b00000100, 0b00001000, // 6
    0b00010000, 0b00100000, 0b01000000, 0b10000000, 0b00011011, 0b00110110, 0b01101100, 0b11011000, // 7
    0b10101011, 0b01001101, 0b10011010, 0b00101111, 0b01011110, 0b10111100, 0b1100011, 0b11011000,  // 8
    0b10010111, 0b01110101, 0b1101010, 0b11010011, 0b10110011, 0b01111101, 0b11111010, 0b11101111,  // 9
    0b11000101, 0b10010001, 0b00111001, 0b01110010, 0b11100100, 0b11010011, 0b10111101, 0b01100001, // A
    0b11000010, 0b10011111, 0b00100101, 0b01001010, 0b10010100, 0b00110011, 0b01100110, 0b11001100, // B
    0b10000011, 0b00011101, 0b00111010, 0b01110100, 0b11101000, 0b11001011, 0b10001101, 0b00000001, // C
    0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000, 0b10000000, 0b00011011, // D
    0b00110110, 0b01101100, 0b11011000, 0b10101011, 0b01001101, 0b10011010, 0b00101111, 0b01011110, // E
    0b10111100, 0b1100011, 0b11011000, 0b10010111, 0b01110101, 0b1101010, 0b11010011, 0b10110011    // F
};

// Hàm thực hiện hoán vị byte theo S-box
void subBytes(std::bitset<128> &key)
{
    std::bitset<128> new_key;
    for (int i = 0; i < 16; ++i)
    {
        int byte_index = i * 8;
        std::bitset<128> byte = (key >> byte_index) & std::bitset<128>(0xFF);
        new_key |= std::bitset<128>(sbox[byte.to_ulong()]) << byte_index;
    }
    key = new_key;
}

// Hàm sinh khóa k dựa trên Rcon
std::bitset<128> generateKey(const std::bitset<128> &key)
{
    std::bitset<128> new_key = key;
    for (int i = 0; i < 11; ++i)
    {
        subBytes(new_key);                    // Hoán vị byte theo S-box
        new_key ^= std::bitset<128>(rcon[i]); // XOR với Rcon
    }
    return new_key;
}

std::vector<std::bitset<128>> divideMessageIntoBlocks(const std::string &message)
{
    std::vector<std::bitset<128>> blocks;
    const size_t block_size = 16; // 16 characters * 8 bits/character = 128 bits

    // Lặp qua từng phần của tin nhắn và chia thành các khối 128 bit
    for (size_t i = 0; i < message.length(); i += block_size)
    {
        std::string block_str = message.substr(i, block_size);
        // Nếu độ dài không đủ, thêm ký tự space hoặc thực hiện xử lý phù hợp
        while (block_str.length() < block_size)
        {
            block_str += ' '; // Thêm space nếu độ dài không đủ
        }

        std::bitset<128> block;
        for (size_t j = 0; j < block_str.length(); ++j)
        {
            block <<= 8;
            block |= std::bitset<128>(block_str[j]);
        }
        blocks.push_back(block);
    }
    return blocks;
}

std::bitset<128> encryptBlock(const std::bitset<128> &generated_key, const std::bitset<128> &block)
{
    return generated_key ^ block; // Simple XOR encryption for demonstration purposes
}
std::bitset<128> decryptBlock(const std::bitset<128> &generated_key, const std::bitset<128> &encrypted_block)
{
    return generated_key ^ encrypted_block; // Simple XOR decryption for demonstration purposes
}
std::string blocksToMessage(const std::vector<std::bitset<128>> &message_blocks, size_t original_length)
{
    std::string message;
    const size_t block_size = 16; // 16 characters * 8 bits/character = 128 bits

    for (const auto &block : message_blocks)
    {
        std::string block_str;
        for (size_t i = 0; i < block_size; ++i)
        {
            // Handle each 8 bits (1 character) individually
            char ch;
            if (i < 8)
            { // First 64 bits
                ch = static_cast<char>((block >> (i * 8)).to_ullong() & 0xFF);
            }
            else
            { // Next 64 bits
                ch = static_cast<char>((block >> (i * 8 - 64)).to_ullong() & 0xFF);
            }
            block_str = ch + block_str; // Prepend the character because we're reading bits in reverse order
        }
        message += block_str;
    }

    // Only take as many characters as were in the original message
    message = message.substr(0, original_length);

    return message;
}

int main()
{
    // Khóa gốc, ví dụ: tạo một khóa ngẫu nhiên
    std::bitset<128> original_key(100101011010010101);

    // Sinh khóa k
    std::bitset<128> generated_key = generateKey(original_key);

    // In ra khóa đã sinh
    std::cout << "Generated key K: " << generated_key << std::endl;

    std::string message = "this is a test"; // Tin nhắn cần chia thành các khối

    // Chia tin nhắn thành các khối 128 bit
    std::vector<std::bitset<128>> message_blocks = divideMessageIntoBlocks(message);

    // In ra các khối tin nhắn 128 bit
    for (const auto &block : message_blocks)
    {
        std::cout << "Block: " << block << std::endl;
    }
    for (const auto &block : message_blocks)
    {
        std::bitset<128> encrypted_block = encryptBlock(generated_key, block);
        std::cout << "Encrypted Block: " << encrypted_block << std::endl;

        // Decrypt each block using the same key
        std::bitset<128> decrypted_block = decryptBlock(generated_key, encrypted_block);
        std::cout << "Decrypted Block: " << decrypted_block << std::endl;
    }
    // Chuyển các khối đã giải mã thành tin nhắn ban đầu
    std::string decrypted_message = blocksToMessage(message_blocks, message.length());
    std::cout << "Decrypted Message: " << decrypted_message << std::endl;

    return 0;
}
