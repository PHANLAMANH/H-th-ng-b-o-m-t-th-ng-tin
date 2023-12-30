#include "Header.h"

// nothing here

// S-box và Rcon đã được cung cấp

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
// Hàm ShiftRows tương thích với mảng mul2, mul3
void shiftRows(std::bitset<128> &block)
{
    std::bitset<128> tmp_block = block;

    // Shift row 1 (index 0-3) by 1 byte to the left
    for (int i = 0; i < 4; ++i)
    {
        std::bitset<8> byte = std::bitset<8>((block >> (i * 32) & std::bitset<128>(0xFF)).to_ulong());
        byte = (byte << 2) | (byte >> 6); // Shift left by 1 byte
        tmp_block |= (std::bitset<128>(byte.to_ulong()) << (i * 32));
    }

    // Shift row 2 (index 4-7) by 2 bytes to the left
    for (int i = 4; i < 8; ++i)
    {
        std::bitset<8> byte = std::bitset<8>((block >> (i * 32) & std::bitset<128>(0xFF)).to_ulong());
        byte = (byte << 4) | (byte >> 4); // Shift left by 2 bytes
        tmp_block |= (std::bitset<128>(byte.to_ulong()) << (i * 32));
    }

    // Shift row 3 (index 8-11) by 3 bytes to the left
    for (int i = 8; i < 12; ++i)
    {
        std::bitset<8> byte = std::bitset<8>((block >> (i * 32) & std::bitset<128>(0xFF)).to_ulong());
        byte = (byte << 6) | (byte >> 2); // Shift left by 3 bytes
        tmp_block |= (std::bitset<128>(byte.to_ulong()) << (i * 32));
    }

    // Row 4 (index 12-15) is not shifted in AES

    block = tmp_block;
}

// Hàm MixColumns tương thích với mảng mul2, mul3
void mixColumns(std::bitset<128> &block, const std::bitset<8> mul2[], const std::bitset<8> mul3[])
{
    std::bitset<128> result;

    for (int i = 0; i < 16; i += 4)
    {
        std::bitset<8> a0 = std::bitset<8>((block >> (i * 8)).to_ulong() & 0xFF);
        std::bitset<8> a1 = std::bitset<8>((block >> ((i + 1) * 8)).to_ulong() & 0xFF);
        std::bitset<8> a2 = std::bitset<8>((block >> ((i + 2) * 8)).to_ulong() & 0xFF);
        std::bitset<8> a3 = std::bitset<8>((block >> ((i + 3) * 8)).to_ulong() & 0xFF);

        std::bitset<8> b0 = mul2[a0.to_ulong()] ^ mul3[a1.to_ulong()] ^ a2 ^ a3;
        std::bitset<8> b1 = a0 ^ mul2[a1.to_ulong()] ^ mul3[a2.to_ulong()] ^ a3;
        std::bitset<8> b2 = a0 ^ a1 ^ mul2[a2.to_ulong()] ^ mul3[a3.to_ulong()];
        std::bitset<8> b3 = mul3[a0.to_ulong()] ^ a1 ^ a2 ^ mul2[a3.to_ulong()];

        result |= (std::bitset<128>(b0.to_ulong()) << (i * 8));
        result |= (std::bitset<128>(b1.to_ulong()) << ((i + 1) * 8));
        result |= (std::bitset<128>(b2.to_ulong()) << ((i + 2) * 8));
        result |= (std::bitset<128>(b3.to_ulong()) << ((i + 3) * 8));
    }

    block = result;
}

// Hàm hoán vị byte ngược lại trong các hàng
void invShiftRows(std::bitset<128> &block)
{
    std::bitset<128> tmp_block = block;

    // Hàng 1 không di chuyển

    // Di chuyển hàng 2 1 byte sang phải
    for (int i = 4; i < 8; ++i)
    {
        int index = i * 16;
        tmp_block[index] = block[index + 8];
        tmp_block[index + 8] = block[index + 4];
    }

    // Di chuyển hàng 3 2 byte sang phải
    for (int i = 8; i < 12; ++i)
    {
        int index = i * 16;
        tmp_block[index] = block[index + 16];
        tmp_block[index + 8] = block[index + 24];
        tmp_block[index + 16] = block[index + 8];
        tmp_block[index + 24] = block[index];
    }

    // Hàng 4 di chuyển 3 byte sang phải
    for (int i = 12; i < 16; ++i)
    {
        int index = i * 16;
        tmp_block[index] = block[index + 24];
        tmp_block[index + 8] = block[index + 16];
        tmp_block[index + 16] = block[index + 8];
        tmp_block[index + 24] = block[index];
    }

    block = tmp_block;
}

// Hàm ngược lại của việc thay thế byte theo S-box
void invSubBytes(std::bitset<128> &block)
{
    std::bitset<128> tmp_block = block;
    for (int i = 0; i < 16; ++i)
    {
        int byte_index = i * 8;
        std::bitset<128> byte = (block >> byte_index) & std::bitset<128>(0xFF);
        tmp_block |= std::bitset<128>(inv_sbox[byte.to_ulong()]) << byte_index;
    }
    block = tmp_block;
}

// // Hàm thực hiện ngược lại việc MixColumns
// void invMixColumns(std::bitset<128> &block)
// {
//     for (int i = 0; i < 16; i += 4)
//     {
//         std::bitset<8> col[4];
//         for (int j = 0; j < 4; ++j)
//         {
//             int index = i * 8 + j * 32;
//             col[j] = std::bitset<8>((block >> index & std::bitset<128>(0xFF)).to_ulong());
//         }

//         std::bitset<8> tmp[4];
//         tmp[0] = mul14[col[0].to_ulong()] ^ mul11[col[1].to_ulong()] ^ mul13[col[2].to_ulong()] ^ mul9[col[3].to_ulong()];
//         tmp[1] = mul9[col[0].to_ulong()] ^ mul14[col[1].to_ulong()] ^ mul11[col[2].to_ulong()] ^ mul13[col[3].to_ulong()];
//         tmp[2] = mul13[col[0].to_ulong()] ^ mul9[col[1].to_ulong()] ^ mul14[col[2].to_ulong()] ^ mul11[col[3].to_ulong()];
//         tmp[3] = mul11[col[0].to_ulong()] ^ mul13[col[1].to_ulong()] ^ mul9[col[2].to_ulong()] ^ mul14[col[3].to_ulong()];

//         std::bitset<32> mixed_column;
//         for (int j = 0; j < 4; ++j)
//         {
//             mixed_column |= std::bitset<32>(tmp[j].to_ulong()) << (j * 8);
//         }

//         for (int j = 0; j < 4; ++j)
//         {
//             int index = i * 8 + j * 32;
//             std::bitset<32> small_bitset = mixed_column & std::bitset<32>(0xFF);
//             std::string bitset_string = small_bitset.to_string();
//             std::bitset<128> large_bitset(bitset_string);
//             block ^= large_bitset << index;
//             mixed_column >>= 8;
//         }
//     }
// }
void invMixColumns(std::bitset<128> &block)
{
    for (int i = 0; i < 16; i += 4)
    {
        std::bitset<8> col[4];
        for (int j = 0; j < 4; ++j)
        {
            int index = (i + j) * 8;
            col[j] = std::bitset<8>((block >> index).to_ulong() & 0xFF);
        }

        std::bitset<8> tmp[4];
        tmp[0] = mul14[col[0].to_ulong()] ^ mul11[col[1].to_ulong()] ^ mul13[col[2].to_ulong()] ^ mul9[col[3].to_ulong()];
        tmp[1] = mul9[col[0].to_ulong()] ^ mul14[col[1].to_ulong()] ^ mul11[col[2].to_ulong()] ^ mul13[col[3].to_ulong()];
        tmp[2] = mul13[col[0].to_ulong()] ^ mul9[col[1].to_ulong()] ^ mul14[col[2].to_ulong()] ^ mul11[col[3].to_ulong()];
        tmp[3] = mul11[col[0].to_ulong()] ^ mul13[col[1].to_ulong()] ^ mul9[col[2].to_ulong()] ^ mul14[col[3].to_ulong()];

        std::bitset<32> mixed_column;
        for (int j = 0; j < 4; ++j)
        {
            mixed_column |= std::bitset<32>(tmp[j].to_ulong()) << (j * 8);
        }

        for (int j = 0; j < 4; ++j)
        {
            int index = (i + j) * 8;
            std::bitset<32> small_bitset = mixed_column & std::bitset<32>(0xFF);
            block ^= std::bitset<128>(small_bitset.to_ulong()) << index;
            mixed_column >>= 8;
        }
    }
}

std::bitset<128> generateKey(const std::bitset<128> &key)
{
    std::bitset<128> new_key = key;

    for (int i = 0; i < 10; ++i)
    {
        // Thực hiện các bước chính của Key Expansion trong AES
        subBytes(new_key);
        shiftRows(new_key);
        // mixColumns(new_key, mul2, mul3);

        // XOR với Rcon
        std::bitset<128> rcon_byte = std::bitset<128>(rcon[i]);

        // XOR với phần tử trước 4 bytes (word trước) của new_key
        std::bitset<128> tmp_key = new_key;
        tmp_key >>= 32; // Lấy 4 byte thấp nhất
        new_key ^= tmp_key;
    }

    // Thực hiện một lần nữa SubBytes cho new_key cuối cùng trước khi trả về
    subBytes(new_key);

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
    std::bitset<128> round_key = generated_key;

    // AddRoundKey: XOR khối dữ liệu với khóa ban đầu
    std::bitset<128> encrypted_block = block ^ round_key;

    // 9 vòng lặp cho AES-128 (có thể điều chỉnh cho AES khác)
    for (int i = 0; i < 9; ++i)
    {
        subBytes(encrypted_block);
        shiftRows(encrypted_block);
        // mixColumns(encrypted_block, mul2, mul3);
        round_key = generateKey(round_key); // Tạo khóa cho vòng lặp tiếp theo
        encrypted_block ^= round_key;       // AddRoundKey: XOR với khóa vòng tiếp theo
    }

    // Vòng lặp cuối cùng mà không có MixColumns
    subBytes(encrypted_block);
    shiftRows(encrypted_block);
    round_key = generateKey(round_key);
    encrypted_block ^= round_key;

    return encrypted_block;
}

std::bitset<128> decryptBlock(const std::bitset<128> &generated_key, const std::bitset<128> &encrypted_block)
{
    std::bitset<128> round_key = generated_key;

    // Vòng lặp cuối cùng mà không có MixColumns (ngược lại với encryptBlock)
    std::bitset<128> decrypted_block = encrypted_block ^ generateKey(round_key);

    // 9 vòng lặp ngược lại cho AES-128 (có thể điều chỉnh cho AES khác)
    for (int i = 8; i >= 0; --i)
    {
        invShiftRows(decrypted_block);
        invSubBytes(decrypted_block);
        round_key = generateKey(round_key); // Tạo khóa cho vòng lặp tiếp theo
        decrypted_block ^= round_key;       // AddRoundKey: XOR với khóa vòng tiếp theo
        // invMixColumns(decrypted_block);     // Giải mã MixColumns
    }

    // AddRoundKey cuối cùng với khóa ban đầu
    invShiftRows(decrypted_block);
    invSubBytes(decrypted_block);
    round_key = generateKey(round_key);
    decrypted_block ^= round_key;

    return decrypted_block;
}

std::string blocksToMessage(const std::vector<std::bitset<128>> &message_blocks, size_t original_length)
{
    std::string message;

    for (const auto &block : message_blocks)
    {
        std::string block_str = block.to_string();
        for (size_t i = 0; i < block_str.size(); i += 8)
        {
            std::bitset<8> byte(block_str.substr(i, 8));
            message += static_cast<char>(byte.to_ulong());
        }
    }

    // Chỉ lấy số ký tự ban đầu của tin nhắn
    message = message.substr(0, original_length);

    return message;
}

// int main()
// {
//     // generate original key using random number generator bitset<128> binary
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_int_distribution<> dis(0, 1);
//     std::bitset<128> original_key;
//     for (int i = 0; i < 16; ++i)
//     {
//         original_key[i] = dis(gen);
//     }
//     std::cout << "Original key: " << original_key << std::endl;

//     // Sinh khóa k
//     std::bitset<128> generated_key = generateKey(original_key);

//     // đọc file mess.txt và lưu vào message
//     std::ifstream inFile("mess.txt");
//     if (!inFile)
//     {
//         std::cerr << "Error opening input file: mess.txt" << std::endl;
//         return 1;
//     }
//     std::string message;
//     std::getline(inFile, message);
//     inFile.close();

//     // Chia tin nhắn thành các khối 128 bit
//     std::vector<std::bitset<128>> message_blocks = divideMessageIntoBlocks(message);

//     // Mã hóa các khối tin nhắn và in các khối văn bản mã hóa ra file encrypted_aes.txt
//     std::ofstream outFile("encrypted_aes.txt");
//     if (!outFile)
//     {
//         std::cerr << "Error opening output file: encrypted_aes.txt" << std::endl;
//         return 1;
//     }

//     for (const auto &block : message_blocks)
//     {
//         std::bitset<128> encrypted_block = encryptBlock(generated_key, block);
//         outFile << encrypted_block << std::endl;
//     }
//     outFile.close();

//     // Đọc file encrypted_aes.txt, giải mã các khối và in ra màn hình
//     std::ifstream inFile2("encrypted_aes.txt");
//     if (!inFile2)
//     {
//         std::cerr << "Error opening input file: encrypted_aes.txt" << std::endl;
//         return 1;
//     }

//     std::vector<std::bitset<128>> encrypted_blocks;
//     std::string line;
//     while (std::getline(inFile2, line))
//     {
//         encrypted_blocks.push_back(std::bitset<128>(line));
//     }
//     inFile2.close();

//     // Chuyển các khối đã mã hóa thành tin nhắn ban đầu và in vào decrypted_aes.txt
//     std::ofstream outFile2("decrypted_aes.txt");
//     if (!outFile2)
//     {
//         std::cerr << "Error opening output file: decrypted_aes.txt" << std::endl;
//         return 1;
//     }

//     for (const auto &block : encrypted_blocks)
//     {
//         std::bitset<128> decrypted_block = decryptBlock(generated_key, block);
//         // sử dụng hàm blocksToMessage để chuyển các khối đã giải mã thành tin nhắn ban đầu
//         std::string decrypted_message = blocksToMessage(message_blocks, message.length());
//         outFile2 << decrypted_message << std::endl;
//     }
//     outFile2.close();

//     return 0;
// }