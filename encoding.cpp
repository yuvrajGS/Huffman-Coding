#include <fstream>
#include <iostream>

using std::string;
struct node {
    char ch;
    int freq;
    int code;
    node *left;
    node *right;
    node() {}
    node(char character, int frequency, int c) : ch(character), freq(frequency), code(c) {}
    bool operator>(const node &rhs) {
        return this->freq > rhs.freq;
    }
    bool operator<(const node &rhs) {
        return this->freq < rhs.freq;
    }
};

void swap(node *x, node *y) {
    node temp = *x;
    *x = *y;
    *y = temp;
}

class heap {
private:
    node *pQueue;
    int hCap;
    int hSize;
    int parent(int i) {
        return (i - 1) / 2;
    }
    int left(int i) {
        return (2 * i + 1);
    }
    int right(int i) {
        return (2 * i + 2);
    }

public:
    heap(int capacity, int size) : pQueue(new node[capacity]), hCap(capacity), hSize(size) {}
    void heapify(int i) {
        int l = left(i);
        int r = right(i);
        int smallest = i;
        if (l < hSize && pQueue[l] < pQueue[i])
            smallest = l;
        if (r < hSize && pQueue[r] < pQueue[smallest])
            smallest = r;
        if (smallest != i) {
            swap(&pQueue[i], &pQueue[smallest]);
            heapify(smallest);
        }
    }
    node *popRoot() {
        if (hSize <= 0)
            return nullptr;
        if (hSize == 1) {
            hSize--;
            return &pQueue[0];
        }
        node* root = &pQueue[0];
        pQueue[0] = pQueue[hSize - 1];
        hSize--;
        heapify(0);

        return root;
    }
    node *getRoot() {
        return &pQueue[0];
    }
    void insert(node *newNode) {
        hSize++;
        int index = hSize - 1;
        pQueue[index] = *newNode; // insert at end

        while (index != 0 && (pQueue[parent(index)] > pQueue[index])) {
            swap(&pQueue[index], &pQueue[parent(index)]);
            index = parent(index);
        }
    }
    ~heap(){
        delete[] pQueue;
    }
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