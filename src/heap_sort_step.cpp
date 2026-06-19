// *********************************************************
// Program: heap_sort_step.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC3L
// Tutorial Class: TT10L
// Trimester: 2610
// Member_1: ID | NAME | EMAIL | PHONE
// Member_2: 242UC244PD | Tan Yi Ling | tan.yi.ling1@student.edu.my | 0182094439
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

using namespace std;

// Structure to hold each record
struct Record {
    long long integer;
    string str;
};

// -------------------------------------------------------
// PRINT ARRAY STATE TO FILE
// Format: [int/str, int/str, ...] label
// -------------------------------------------------------
void printState(ofstream& outFile, const vector<Record>& arr, const string& label) {
    outFile << "[";
    for (int i = 0; i < (int)arr.size(); i++) {
        outFile << arr[i].integer << "/" << arr[i].str;
        if (i < (int)arr.size() - 1) outFile << ", ";
    }
    outFile << "] " << label << "\n";
}

// -------------------------------------------------------
// MAX HEAPIFY
// -------------------------------------------------------
void heapify(vector<Record>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].integer > arr[largest].integer)
        largest = left;

    if (right < n && arr[right].integer > arr[largest].integer)
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

// -------------------------------------------------------
// HEAP SORT STEP BY STEP
// Logs every step to output file
// Matches sample output format from assignment
// -------------------------------------------------------
void heapSortStep(vector<Record>& arr, ofstream& outFile) {
    int n = arr.size();

    // Step 1: Build max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Print initial state (after building max heap)
    printState(outFile, arr, "initial");

    // Step 2: Extract elements one by one
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);

        // Print state after each extraction
        // Label: i = <current index>
        printState(outFile, arr, "i = " + to_string(i));
    }
}

// -------------------------------------------------------
// READ CSV FILE
// -------------------------------------------------------
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
        string intPart, strPart;
        if (getline(ss, intPart, ',') && getline(ss, strPart)) {
            Record r;
            r.integer = stoll(intPart);
            r.str = strPart;
            data.push_back(r);
        }
    }
    file.close();
    return data;
}

// -------------------------------------------------------
// MAIN
// Usage: ./heap_sort_step <dataset.csv> <start_row> <end_row>
// -------------------------------------------------------
int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: ./heap_sort_step <dataset.csv> <start_row> <end_row>" << endl;
        cerr << "Example: ./heap_sort_step dataset_1000.csv 1 7" << endl;
        return 1;
    }

    string inputFile = argv[1];
    int startRow = stoi(argv[2]);
    int endRow = stoi(argv[3]);

    // Extract dataset size from filename
    string size = "1000";
    size_t s = inputFile.find('_');
    size_t e = inputFile.find('.');
    if (s != string::npos && e != string::npos)
        size = inputFile.substr(s + 1, e - s - 1);

    // Output filename format from assignment:
    // dataset_1000_heap_sorted_step_startrow_endrow.txt
    string outputFile = "dataset_" + size + "_heap_sorted_step_" +
                        to_string(startRow) + "_" + to_string(endRow) + ".txt";

    // Read all data
    vector<Record> allData = readCSV(inputFile);

    if (allData.empty()) {
        cerr << "Error: No data read." << endl;
        return 1;
    }

    // Validate row range (1-indexed)
    if (startRow < 1 || endRow > (int)allData.size() || startRow > endRow) {
        cerr << "Error: Invalid row range. Dataset has " << allData.size() << " rows." << endl;
        return 1;
    }

    // Extract subset from start_row to end_row (1-indexed)
    vector<Record> subset(allData.begin() + startRow - 1, allData.begin() + endRow);

    // Open output file
    ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        cerr << "Error: Cannot write to " << outputFile << endl;
        return 1;
    }

    // Run heap sort step by step
    heapSortStep(subset, outFile);

    outFile.close();
    cout << "Step-by-step output saved to " << outputFile << endl;

    return 0;
}