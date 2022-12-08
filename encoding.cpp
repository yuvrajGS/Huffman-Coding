/*
Yuvraj Singh
200670570
*/

#include <fstream>
#include <iostream>
#include <string.h>
#include <unordered_map>
#include <vector>

using std::string;
struct node {
    char ch;
    int freq=0;
    node *left =nullptr;
    node *right=nullptr;
    node(char character) : ch(character){}
};
class heap {
private:
    std::vector<node *> pQueue;
    int parent(int i) {
        return (i - 1) / 2;
    }

    int left(int i) {
        return (2 * i + 1);
    }

    int right(int i) {
        return (2 * i + 2);
    }
    void heapify_down(int i) {
        int l = left(i);
        int r = right(i);
        int smallest = i;
        if (l < size() && pQueue[l]->freq < pQueue[i]->freq) {
            smallest = l;
        }
        if (r < size() && pQueue[r]->freq < pQueue[smallest]->freq) {
            smallest = r;
        }
        if (smallest != i) {
            swap(pQueue[i], pQueue[smallest]);
            heapify_down(smallest);
        }
    }
    void heapify_up(int i) {
        if (i && pQueue[parent(i)]->freq > pQueue[i]->freq) {
            swap(pQueue[i], pQueue[parent(i)]);
            heapify_up(parent(i));
        }
    }
    void swap(node *i, node *j) {
        node temp = *i;
        *i = *j;
        *j = temp;
    }

public:
    heap() {}
    node *popRoot() {
        node *temp = pQueue.at(0);
        pQueue[0] = pQueue.back();
        pQueue.pop_back();
        heapify_down(0);
        return temp;
    }
    void insert(node *newNode) {
        pQueue.push_back(newNode);
        heapify_up(pQueue.size() - 1);
    }
    int size() {
        return pQueue.size();
    }
};

void readFile(string &filename, std::unordered_map<char, node *> &freqMap) {
    std::ifstream myfile(filename);
    if (myfile.is_open()) {
        char ch;
        while (myfile >> std::noskipws >> ch) {
            ch = tolower(ch);
            if ((ch <= 'z' && ch >= 'a') || isspace(ch) || (ch >= '0' && ch <= '9') || ch == ',' || ch == '.') {
                if (isspace(ch))
                    ch = ' ';
                ++freqMap[ch]->freq; // increment freq
            }
        }
        myfile.close();
    } else
        std::cout << "Unable to open file\n";
}
void genCodes_AUX(node *n, int codes[], int index, std::ofstream &myFile) {
    if (n->left) {
        codes[index] = 1;
        genCodes_AUX(n->left, codes, index + 1, myFile);
    }
    if (!(n->left) && !(n->right)) {
        myFile << n->ch << ":";
        for (int i = 0; i < index; ++i)
            myFile << codes[i];
        myFile << '\n';
    }
    if (n->right) {
        codes[index] = 0;
        genCodes_AUX(n->right, codes, index + 1, myFile);
    }
}

void genCodes(std::unordered_map<char, node *> &freqMap) {
    int codes_[39], index = 0;
    heap codes;
    node *child1, *child2, *subTree, *root;
    for (auto x : freqMap) {
        codes.insert(x.second);
    }
    // create tree
    while (codes.size() > 1) {
        child1 = codes.popRoot();
        child2 = codes.popRoot();
        subTree = new node('^');
        subTree->freq = child1->freq + child2->freq;
        subTree->left = child1;
        subTree->right = child2;
        codes.insert(subTree);
    }
    root = codes.popRoot(); // pointer to tree
    std::ofstream myFile("codes.txt");
    genCodes_AUX(root, codes_, index, myFile);
    myFile.close();
}
void genCompressed(string &filename) {
    std::unordered_map<char, string> codeMap;
    string line;
    std::ifstream myfile("codes.txt");
    if (myfile.is_open()) {
        char ch;
        while (getline(myfile, line)) { // fill code hashtable with the generated code for each valid character
            ch = tolower(line[0]);
            codeMap[ch] = line.substr(2);
        }
        myfile.close();
    }
    std::ifstream myfile2(filename);
    if (myfile2.is_open()) {
        std::ofstream file("compressed.bin", std::ios::binary);
        char ch, bit=0;
        unsigned char buffer[2048] = {0};
        unsigned char mask = 128;
        int i = 0;
        string code;
        while (myfile2 >> std::noskipws >> ch) { // read each character in test1.txt
            ch = tolower(ch);
            if ((ch <= 'z' && ch >= 'a') || isspace(ch) || (ch >= '0' && ch <= '9') || ch == ',' || ch == '.') { // if valid character
                if (isspace(ch))
                    ch = ' ';       // convert tabs,new lines, etc to space
                code = codeMap[ch]; // get code (string) for given character

                for (int k = 0; k < code.size(); k++) { // loop through size of code
                    if (code[k] == '1') {
                        mask = 128;
                        mask = mask >> bit; // right shift mask based on current bit position

                        buffer[i] = buffer[i] | mask; // OR buffer and mask
                        bit++;
                    } else {
                        bit++;
                    }
                    if (bit == 8) { // once character is written to buffer
                        bit = 0;
                        i++;
                    }
                }
                if (i == 2048) { // buffer full, write to binary file
                    file.write((char *)&buffer, 2048);
                    for (int j = 0; j < 2048; j++) {
                        buffer[j] = 0;
                    }
                    i = 0;
                }
            }
        }
        file.write((char *)&buffer, i);
        file.close();
        myfile2.close();
    }
}
/*
int main() {
    string filename, data;
    std::unordered_map<char, node *> freqMap;
    char characters[39] = {' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
                           'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
                           'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', ',', '.'};
    for (char ch : characters) {
        freqMap[ch] = new node(ch);
    }
    // std::cout << "Enter filename to encode: ";
    // std::cin >> filename;
    filename = "test1.txt"; // TESTING
    readFile(filename, freqMap);
    // create frequency.txt
    std::ofstream myFile("frequency.txt");
    for (auto i : freqMap) {
        data = i.first;
        if (data == "\n")
            data = "\\n";
        myFile << data << ":" << i.second->freq << '\n';
    }
    myFile.close();

    genCodes(freqMap);
    genCompressed(filename);
    std::cout << "done" << std::endl;
}
*/