int main(int /* argc */, const char ** /* argv */)
{
    // read input from file mess.txt
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

    // hexToBitset(hash);
    cout << "sha1('" << input << "'):" << hash << endl;
    cout << "sha1('" << input << "'):" << hexToBitset(hash) << endl;
    return 0;
}