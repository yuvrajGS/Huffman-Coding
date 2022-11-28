#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using std::string;
struct node {
    char ch;
    int freq;
    node *left;
    node *right;
};
node *genNode(char character) {
    node *node1 = (node *)malloc(sizeof(node));
    node1->ch = character;
    node1->freq = 0;
    node1->right = nullptr;
    node1->left = nullptr;
    return node1;
}
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
        subTree = genNode('^');
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

int main() {
    string filename, data;
    std::unordered_map<char, node *> freqMap;
    char characters[39] = {' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
                           'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
                           'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', ',', '.'};
    for (char ch : characters) {
        freqMap[ch] = genNode(ch);
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
}