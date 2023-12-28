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