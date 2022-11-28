#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

using std::string;
struct node {
    char ch = ' ';
    int freq = 0;
    string code = "";
    node *left = nullptr;
    node *right = nullptr;
    node() {}
    node(char character) : ch(character) {}
};

class heap {
private:
    std::vector<node> pQueue;
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
        if (l < size() && pQueue[l].freq < pQueue[i].freq) {
            smallest = l;
        }
        if (r < size() && pQueue[r].freq < pQueue[smallest].freq) {
            smallest = r;
        }
        if (smallest != i) {
            swap(i, smallest);
            heapify_down(smallest);
        }
    }
    void heapify_up(int i) {
        if (i && pQueue[parent(i)].freq > pQueue[i].freq) {
                swap(i, parent(i));
                heapify_up(parent(i));
            }
    }
    void swap(int i, int j) {
        node temp = pQueue.at(i);
        pQueue[i] = pQueue[j];
        pQueue[j] = temp;
    }

public:
    heap() {}
    node popRoot() {
        node temp = (pQueue.at(0));
        pQueue[0] = pQueue.back();
        pQueue.pop_back();
        heapify_down(0);
        return temp;
    }
    void insert(node newNode) {
        pQueue.push_back(newNode);
        heapify_up(pQueue.size() - 1);
    }
    int size() {
        return pQueue.size();
    }
};

void readFile(string &filename, std::unordered_map<char, node> &freqMap) {
    std::ifstream myfile(filename);
    if (myfile.is_open()) {
        char ch;
        while (myfile >> std::noskipws >> ch) {
            ch = tolower(ch);
            if ((ch <= 'z' && ch >= 'a') || isspace(ch) || (ch >= '0' && ch <= '9') || ch == ',' || ch == '.') {
                if (isspace(ch))
                    ch = ' ';
                ++freqMap[ch].freq; // increment freq
            }
        }
        myfile.close();
    } else
        std::cout << "Unable to open file\n";
}
void genCodes_AUX(node *n, string code) {
    if ((*n).left != nullptr) {
        code.append("0");
        std::cout << (*n).ch << (*n).freq << std::endl;
        genCodes_AUX((*n).left, code);
    }
    if ((*n).right != nullptr) {
        code.append("1");
        genCodes_AUX(n->right, code);
    }
    if ((*n).ch != '^') {
        std::cout << "test" << std::endl;
    }
}
void genCodes(std::unordered_map<char, node> &freqMap) {
    string c = "";
    heap codes;
    for (auto x : freqMap) {
        codes.insert(x.second);
    }
    // create tree
    std::vector<node> temp;
    while (codes.size() > 1) {
        temp.emplace_back(codes.popRoot());
        temp.emplace_back(codes.popRoot());
        node subTree = node('^');
        subTree.freq = temp[temp.size()-1].freq + temp[temp.size()-2].freq;
        subTree.left = &temp[temp.size()-2];
        subTree.right = &temp[temp.size()-1];
        codes.insert(subTree);
    }
    node root = codes.popRoot(); // pointer to tree
    genCodes_AUX(&root, c);
    std::cout << "test" << std::endl;
}

int main() {
    string filename, data;
    std::unordered_map<char, node> freqMap;
    char characters[39] = {' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
                           'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
                           'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', ',', '.'};
    for (char ch : characters) {
        freqMap[ch] = node(ch);
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
        myFile << data << ":" << i.second.freq << '\n';
    }
    myFile.close();

    genCodes(freqMap);
}