// *********************************************************
// Program: hash_table_search.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC3L
// Tutorial Class: TT10L
// Trimester: 2610
// Member_1: ID | NAME | EMAIL | PHONE
// Member_2: 242UC244PD | Tan Yi Ling | 242UC244PD | 0182094439
// Member_3: 242UC244K8 | Sweeney Chaw Hui Shi | sweeney.chaw.hui@student.mmu.edu.my | 011-26792612
// 
// *********************************************************
// Task Distribution
// Member_1: Dataset Generator + Radix Sort
// Member_2: Heap Sort
// Member_3: Hash Table Search
// 
// *********************************************************

#include <chrono>
#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Record structure for dataset rows
struct Record {
    long long id;
    string word;
};

// Hash node for separate chaining
struct HashNode {
    long long key;
    string value;
    HashNode* next;

    HashNode(long long k, const string& v)
        : key(k), value(v), next(nullptr) {}
};

// Hash table implementation with separate chaining
class HashTable {
public:
    int capacity;
    vector<HashNode*> table;

    HashTable(int cap) : capacity(cap), table(cap, nullptr) {}

    ~HashTable() {
        for (int i = 0; i < capacity; i++) {
            HashNode* cur = table[i];
            while (cur) {
                HashNode* tmp = cur;
                cur = cur->next;
                delete tmp;
            }
        }
    }

    int hashFunction(long long key) const {
        return static_cast<int>(key % capacity);
    }

    void insert(long long key, const string& value) {
        int idx = hashFunction(key);
        HashNode* node = new HashNode(key, value);
        node->next = table[idx];
        table[idx] = node;
    }

    HashNode* search(long long key) const {
        int idx = hashFunction(key);
        HashNode* cur = table[idx];
        while (cur) {
            if (cur->key == key)
                return cur;
            cur = cur->next;
        }
        return nullptr;
    }
};

bool isPrime(int n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; static_cast<long long>(i) * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}

int nextPrime(int n) {
    while (!isPrime(n)) n++;
    return n;
}

vector<Record> readCSV(const string& filename) {
    vector<Record> data;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return data;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string idPart, wordPart;
        if (getline(ss, idPart, ',') && getline(ss, wordPart)) {
            Record r;
            r.id = stoll(idPart);
            r.word = wordPart;
            data.push_back(r);
        }
    }

    file.close();
    return data;
}

string extractSize(const string& filename) {
    size_t start = filename.find('_');
    size_t end = filename.find('.');
    if (start == string::npos || end == string::npos) return "unknown";
    return filename.substr(start + 1, end - start - 1);
}

string makeOutputFile(const string& inputFile, const string& baseName) {
    size_t pos = inputFile.find_last_of("/\\");
    string dir = (pos == string::npos) ? "" : inputFile.substr(0, pos + 1);
    return dir + baseName;
}

long long findWorstCaseKey(const HashTable& ht) {
    int maxLen = 0;
    int maxBucket = 0;
    for (int i = 0; i < ht.capacity; i++) {
        int len = 0;
        HashNode* cur = ht.table[i];
        while (cur) {
            len++;
            cur = cur->next;
        }
        if (len > maxLen) {
            maxLen = len;
            maxBucket = i;
        }
    }

    HashNode* cur = ht.table[maxBucket];
    while (cur && cur->next)
        cur = cur->next;

    return cur ? cur->key : -1LL;
}

long long findBestCaseKey(const HashTable& ht) {
    for (int i = 0; i < ht.capacity; i++) {
        if (ht.table[i] && ht.table[i]->next == nullptr)
            return ht.table[i]->key;
    }

    for (int i = 0; i < ht.capacity; i++) {
        if (ht.table[i])
            return ht.table[i]->key;
    }
    return -1LL;
}

double measureRepeatedSearch(const HashTable& ht, long long key, int repeats) {
    auto tStart = chrono::high_resolution_clock::now();
    for (int i = 0; i < repeats; i++)
        ht.search(key);
    auto tEnd = chrono::high_resolution_clock::now();
    return chrono::duration<double>(tEnd - tStart).count();
}

double measureSearchList(const HashTable& ht, const vector<long long>& keys) {
    auto tStart = chrono::high_resolution_clock::now();
    for (long long key : keys)
        ht.search(key);
    auto tEnd = chrono::high_resolution_clock::now();
    return chrono::duration<double>(tEnd - tStart).count();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: ./hash_table_search <dataset_filename.csv>" << endl;
        cerr << "Example: ./hash_table_search dataset_1000.csv" << endl;
        return 1;
    }

    string inputFile = argv[1];
    string size = extractSize(inputFile);
    string outputFile = makeOutputFile(inputFile, "hash_table_search_dataset_" + size + ".txt");

    cout << "Reading dataset from " << inputFile << "..." << endl;
    vector<Record> data = readCSV(inputFile);
    if (data.empty()) {
        cerr << "Error: No data read from file." << endl;
        return 1;
    }

    int n = static_cast<int>(data.size());
    cout << "Dataset size: " << n << " elements" << endl;

    int tableSize = nextPrime(static_cast<int>(n * 1.5));
    HashTable ht(tableSize);
    for (const Record& r : data)
        ht.insert(r.id, r.word);

    cout << "Hash table built. Capacity: " << tableSize
         << "  Load factor: " << static_cast<double>(n) / tableSize << endl;

    long long bestKey = findBestCaseKey(ht);
    long long worstKey = findWorstCaseKey(ht);

    vector<long long> averageKeys;
    averageKeys.reserve(n);
    for (const Record& r : data)
        averageKeys.push_back(r.id);

    double bestTime = measureRepeatedSearch(ht, bestKey, n);
    double avgTime = measureSearchList(ht, averageKeys);
    double worstTime = measureRepeatedSearch(ht, worstKey, n);

    cout << "\n--- Hash Table Search Running Times (n = " << n << " searches) ---" << endl;
    cout << "Best  case time : " << bestTime << " seconds" << endl;
    cout << "Average case time: " << avgTime << " seconds" << endl;
    cout << "Worst case time : " << worstTime << " seconds" << endl;

    ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        cerr << "Error: Cannot write to file " << outputFile << endl;
        return 1;
    }

    outFile << "Hash Table Search - Dataset size: " << n << "\n";
    outFile << "Table capacity   : " << tableSize << "\n";
    outFile << "Load factor      : " << static_cast<double>(n) / tableSize << "\n\n";
    outFile << "Best  case time : " << bestTime << " seconds\n";
    outFile << "Average case time: " << avgTime << " seconds\n";
    outFile << "Worst case time : " << worstTime << " seconds\n\n";
    outFile << "Notes:\n";
    outFile << "- Best case uses a key stored in a bucket with a single node.\n";
    outFile << "- Average case searches every key from the dataset once.\n";
    outFile << "- Worst case uses a key located in the longest collision chain.\n";
    outFile << "- Timings exclude I/O and only measure search execution.\n";
    outFile.close();

    cout << "\nDone! Output saved to " << outputFile << endl;
    return 0;
}
