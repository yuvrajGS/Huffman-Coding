#include <fstream>
#include <iostream>

using std::string;
struct node {
    char ch;
    int freq;
    int code;
};

void readFile(string &filename, string &data) {
    std::ifstream myfile(filename);
    if (myfile.is_open()) {
        char ch;
        while (myfile >> std::noskipws >> ch) {
            if (isupper(ch))
                ch = tolower(ch);
            if ((ch <= 'z' && ch >= 'a') || ch == ' ' || (ch >= '0' && ch <= '9') || ch == '\n' || ch == ',' || ch == '.')
                data += ch;
        }
        myfile.close();
    } else
        std::cout << "Unable to open file\n";
}

void createFreq(string &data, node freqTable[39]) {
    const int size = data.length();
    std::ofstream myfile;
    myfile.open("frequency.txt");
    char characters[39] = {' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
                           'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
                           'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', ',', '.'};
    for (int i = 0; i < 39; i++) {
        freqTable[i] = node(characters[i], 0, 0);
    }
    myfile.close();
}

int main() {
    string filename, data;
    node freqTable[39];
    std::cout << "Enter filename to encode: ";
    std::cin >> filename;

    readFile(filename, data);
    createFreq(data, freqTable);
    std::cout << data;
}