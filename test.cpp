
#include "sbox_rcon.h"
#include "Header.h"
// int main()
// {
//     const int key_size = 128; // Key size in bits

//     std::random_device rd;
//     std::mt19937_64 gen(rd());
//     std::uniform_int_distribution<uint64_t> dis;

//     uint64_t key_parts[2];
//     key_parts[0] = dis(gen);
//     key_parts[1] = dis(gen);

//     // Combine two 64-bit numbers into a 128-bit key

//     uint64_t combined_key = (key_parts[0] << 64) | key_parts[1];
//     unsigned char aes_key[key_size / 8]; // 128-bit key

//     // Copy the 128-bit key into an array of bytes
//     memcpy(aes_key, &combined_key, key_size / 8);
//     std::cout << key_parts[0] << std::endl;
//     std::cout << key_parts[1] << std::endl;

//     // Display the generated key in hexadecimals
//     std::cout << std::hex << std::uppercase << std::setfill('0');
//     for (int i = 0; i < key_size / 8; ++i)
//     {
//         std::cout << std::setw(2) << static_cast<unsigned>(aes_key[i]);
//     }
//     std::cout << std::endl;

//        return 0;
// }

// AES S-box and Rijndael Rcon tables (for simplicity)
// const uint8_t sbox[128] = {
//     // 0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
//     0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, // 0
//     0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, // 1
//     0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, // 2
//     0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, // 3
//     0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, // 4
//     0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, // 5
//     0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, // 6
//     0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, // 7
//     0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, // 8
//     0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, // 9
//     0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, // A
//     0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, // B
//     0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, // C
//     0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, // D
//     0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, // E
//     0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2  // F
// };
// const uint8_t rcon[128] = {
//     // 0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
//     0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, // 0
//     0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, // 1
//     0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, // 2
//     0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, // 3
//     0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, // 4
//     0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, // 5
//     0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, // 6
//     0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, // 7
//     0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, // 8
//     0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, // 9
//     0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, // A
//     0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, // B
//     0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, // C
//     0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, // D
//     0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, // E
//     0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3  // F
// };
void keyExpansionCore(uint8_t *in, uint8_t i)
{
    // Rotate left
    uint8_t t = in[0];
    in[0] = in[1];
    in[1] = in[2];
    in[2] = in[3];
    in[3] = t;

    // S-Box four bytes
    in[0] = sbox[in[0]];
    in[1] = sbox[in[1]];
    in[2] = sbox[in[2]];
    in[3] = sbox[in[3]];

    // RCon
    in[0] ^= rcon[i];
}
void keyExpansion(const uint8_t *originalKey, uint8_t *expandedKey)
{
    int bytesGenerated = 16; // We've generated 16 bytes so far
    int rconIteration = 1;   // RCon iteration begins at 1
    uint8_t temp[4];         // Temporary storage for a word

    // The first 16 bytes are the original key
    for (int i = 0; i < 16; i++)
    {
        expandedKey[i] = originalKey[i];
    }

    // Generate the rest of the expanded key
    while (bytesGenerated < 176)
    {
        // Read the last word into temp
        for (int i = 0; i < 4; i++)
        {
            temp[i] = expandedKey[(bytesGenerated - 4) + i];
        }

        // Apply core schedule to temp every 16 bytes
        if (bytesGenerated % 16 == 0)
        {
            keyExpansionCore(temp, rconIteration++);
        }

        // XOR temp with the word 16 bytes before the new expanded key. This becomes the next word in the expanded key
        for (uint8_t a = 0; a < 4; a++)
        {
            expandedKey[bytesGenerated] = expandedKey[bytesGenerated - 16] ^ temp[a];
            bytesGenerated++;
        }
    }
}

// AES encryption function
void encryptAES(const uint8_t *input, const uint8_t *key, uint8_t *output)
{
    // This is a placeholder for the AES encryption logic
    // In a real implementation, you would perform the actual AES encryption steps here
    // Steps include AddRoundKey, SubBytes, ShiftRows, MixColumns (for non-final rounds)
    // In this simplified example, I'll just copy the input to the output (no encryption)
}

int main()
{
    const uint8_t originalKey[16] = {"12344678891234"};
    const uint8_t plaintext[128] = {"lmao this is a test message "};
    uint8_t expandedKey[176]; // Expanded key schedule
    uint8_t ciphertext[16];   // Encrypted data buffer

    // Expand the original key into a key schedule
    keyExpansion(originalKey, expandedKey);

    // Encrypt the plaintext using the expanded key schedule
    encryptAES(plaintext, expandedKey, ciphertext);

    // Display the encrypted data (in hexadecimal)
    std::cout << "Encrypted Message (Hex): ";
    for (int i = 0; i < 16; ++i)
    {
        printf("%02x", ciphertext[i]);
    }
    std::cout << std::endl;

    return 0;
}
