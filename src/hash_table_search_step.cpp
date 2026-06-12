// *********************************************************
// Program: hash_table_search_step.cpp
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

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <utility>
 
using namespace std;

//record structure
struct Record {
    long long id;
    string word;
};

//Hash Node - seperate chaining via linked list
struct HashNode {
    long long key;
    string    value;
    HashNode* next;
 
    HashNode(long long k, const string& v)
        : key(k), value(v), next(nullptr) {}
};

//Hash Table class 
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

    //hash function using modulo operator
    int hashFunction(long long key) const {
        return (int)(key % (long long)capacity);
    }

    //insert a key-value pair into the hash table
    void insert(long long key, const string& value) {
        int idx = hashFunction(key);
        HashNode* node = new HashNode(key, value);
        node->next = table[idx];
        table[idx] = node;
    }

    //search for a target key and log the search steps to the output file
    // Perform a single search while logging each comparison to the provided output stream.
    // Returns pair(found, comparisons)
    pair<bool,int> searchWithSteps(long long target, ofstream& outFile) const {
        int idx = hashFunction(target);

        outFile << "Target: " << target << "\n";
        outFile << "Hash index: " << idx << "\n";

        HashNode* cur = table[idx];
        int comparisons = 0;

        if (!cur) {
            // Bucket is empty – instant not-found
            outFile << "-1 != " << target << "\n";
            return {false, 1};
        }

        // Walk the chain, logging each comparison
        while (cur) {
            comparisons++;
            if (cur->key == target) {
                // FOUND
                outFile << cur->key << " = " << cur->key << "/" << cur->value << "\n";
                return {true, comparisons};
            } else {
                // Not a match – log the comparison
                outFile << cur->key << "/" << cur->value
                        << " != " << target << "\n";
            }
            cur = cur->next;
        }

        // Exhausted the chain without finding target
        outFile << "-1 != " << target << "\n";
        return {false, comparisons};
    }
};

//helper functions to find the next prime number for hash table capacity
bool isPrime(int n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; (long long)i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}
 
int nextPrime(int n) {
    while (!isPrime(n)) n++;
    return n;
}

//read csv file 
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
            r.id   = stoll(idPart);
            r.word = wordPart;
            data.push_back(r);
        }
    }
    file.close();
    return data;
}

//extract dataset size from filename
string extractSize(const string& filename) {
    size_t start = filename.find('_');
    size_t end   = filename.find('.');
    if (start == string::npos || end == string::npos) return "unknown";
    return filename.substr(start + 1, end - start - 1);
}

string makeOutputFile(const string& inputFile, const string& baseName) {
    size_t pos = inputFile.find_last_of("/\\");
    string dir = (pos == string::npos) ? "" : inputFile.substr(0, pos + 1);
    return dir + baseName;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: ./hash_table_search_step <dataset.csv> <target_id>" << endl;
        cerr << "Example (found)    : ./hash_table_search_step dataset_1000.csv 2008864030" << endl;
        cerr << "Example (not found): ./hash_table_search_step dataset_1000.csv 123456789" << endl;
        return 1;
    }
 
    string   inputFile = argv[1];
    long long target   = stoll(argv[2]);
    string   size      = extractSize(inputFile);
 
    // Output filename: dataset_<size>_hash_table_search_step_<target>.txt
    string outputFile = makeOutputFile(inputFile,
                        "dataset_" + size + "_hash_table_search_step_" + to_string(target) + ".txt");
 
    
    // Read dataset
    vector<Record> data = readCSV(inputFile);
 
    if (data.empty()) {
        cerr << "Error: No data read from file." << endl;
        return 1;
    }
 
    int n = (int)data.size();
 
    // Build hash table  with a capacity larger than n to reduce collisions
    int tableSize = nextPrime((int)(n * 1.5));
    HashTable ht(tableSize);
 
    for (int i = 0; i < n; i++)
        ht.insert(data[i].id, data[i].word);
 
    // Open output file and run step search while measuring the single-search time
    ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        cerr << "Error: Cannot write to " << outputFile << endl;
        return 1;
    }

    // Header
    outFile << "Dataset file: " << inputFile << "\n";
    outFile << "Dataset size: " << n << "\n";
    outFile << "Table capacity: " << tableSize << "\n";
    outFile << "Load factor: " << static_cast<double>(n) / tableSize << "\n\n";

    auto tStart = chrono::high_resolution_clock::now();
    pair<bool,int> result = ht.searchWithSteps(target, outFile);
    auto tEnd = chrono::high_resolution_clock::now();
    double singleSearchTime = chrono::duration<double>(tEnd - tStart).count();

    // Summary
    outFile << "\nSummary:\n";
    outFile << "Target: " << target << "\n";
    outFile << "Found: " << (result.first ? "Yes" : "No") << "\n";
    outFile << "Comparisons: " << result.second << "\n";
    outFile << "Search time (no I/O): " << singleSearchTime << " seconds\n";

    outFile.close();

    cout << "Step-by-step search output saved to " << outputFile << endl;
 
    return 0;
}