/*
Yuvraj Singh
200670570
*/
#include <fstream>
#include <iostream>
#include <string.h>
#include <vector>

using std::string;

struct node {
    char ch;
    int freq = 0;
    node *left = nullptr;
    node *right = nullptr;
    node(char character) : ch(character) {}
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
    node *getRoot() {
        return pQueue.at(0);
    }
    int size() {
        return pQueue.size();
    }
};

void reconstructHuffman(heap &codes) {
    string line;
    node *child1, *child2, *subTree;
    std::ifstream myfile("frequency.txt");
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            node *ch = new node(line[0]);
            ch->freq = stoi(line.substr(2));
            codes.insert(ch);
        }
        myfile.close();
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
}

void decode(heap &codes) {
    unsigned char mask, ch = 0, temp;
    char bit = 0;
    int j = 0;

    node *tempChar = codes.getRoot();
    std::ofstream myFile("decoded.txt");
    std::ifstream file("compressed.bin", std::ios::binary);
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(file), {});
    file.close();
    while (j != buffer.size()) {
        mask = 128;
        mask = mask >> bit++; // right shift mask based on current bit position
        ch = buffer[j] & mask;
        if (ch) // find code by going through tree
            tempChar = tempChar->left;
        else
            tempChar = tempChar->right;
        if (tempChar->right == nullptr && tempChar->left == nullptr) { // leaf node (node with character) reached
            myFile << tempChar->ch;
            tempChar = codes.getRoot(); // reset node
        }
        if (bit == 8) { // move to next buffer index and reset bit
            j++;
            bit = 0;
        }
    }
    myFile.close();
}

int main() {
    heap codes;
    reconstructHuffman(codes);
    decode(codes);
    std::cout << "Finished" << std::endl;
}
