// *********************************************************
// Program: heap_sort.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC3L
// Tutorial Class: TT10L
// Trimester: 2610
// Member_1: ID | NAME | EMAIL | PHONE
// Member_2: 242UC244PD | Tan Yi Ling | 242UC244PD | 0182094439
// Member_3: ID | NAME | EMAIL | PHONE
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

using namespace std;

// Structure to hold each record
struct Record {
    long long integer;
    string str;
};

// -------------------------------------------------------
// MAX HEAPIFY
// Maintains the max-heap property for subtree rooted at i
// Time Complexity: O(log n)
// -------------------------------------------------------
void heapify(vector<Record>& arr, int n, int i) {
    int largest = i;       // Initialize largest as root
    int left = 2 * i + 1;  // Left child
    int right = 2 * i + 2; // Right child

    // If left child is larger than root
    if (left < n && arr[left].integer > arr[largest].integer)
        largest = left;

    // If right child is larger than current largest
    if (right < n && arr[right].integer > arr[largest].integer)
        largest = right;

    // If largest is not root, swap and continue heapifying
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

// -------------------------------------------------------
// HEAP SORT (using Max Heap)
// Time Complexity: O(n log n) - Best, Average, Worst
// Space Complexity: O(1) - In-place sorting
// -------------------------------------------------------
void heapSort(vector<Record>& arr) {
    int n = arr.size();

    // Step 1: Build max heap
    // Start from last non-leaf node and heapify each node
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Step 2: Extract elements from heap one by one
    for (int i = n - 1; i > 0; i--) {
        // Move current root (max) to end
        swap(arr[0], arr[i]);
        // Heapify the reduced heap
        heapify(arr, i, 0);
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
// WRITE SORTED OUTPUT TO CSV
// -------------------------------------------------------
void writeCSV(const string& filename, const vector<Record>& data) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot write to file " << filename << endl;
        return;
    }
    for (const auto& r : data)
        file << r.integer << "/" << r.str << "\n";
    file.close();
}

// -------------------------------------------------------
// EXTRACT DATASET SIZE FROM FILENAME
// e.g. "dataset_1000.csv" -> "1000"
// -------------------------------------------------------
string extractSize(const string& filename) {
    size_t start = filename.find('_');
    size_t end = filename.find('.');
    if (start == string::npos || end == string::npos) return "unknown";
    return filename.substr(start + 1, end - start - 1);
}

// -------------------------------------------------------
// MAIN
// -------------------------------------------------------
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: ./heap_sort <dataset_filename.csv>" << endl;
        cerr << "Example: ./heap_sort dataset_1000.csv" << endl;
        return 1;
    }

    string inputFile = argv[1];
    string size = extractSize(inputFile);
    string outputFile = "heap_sorted_dataset_" + size + ".csv";

    // Read dataset
    cout << "Reading dataset from " << inputFile << "..." << endl;
    vector<Record> data = readCSV(inputFile);

    if (data.empty()) {
        cerr << "Error: No data read from file." << endl;
        return 1;
    }

    cout << "Dataset size: " << data.size() << " elements" << endl;

    // -------------------------------------------------------
    // START TIMER (excludes I/O time as per assignment)
    // -------------------------------------------------------
    auto start = chrono::high_resolution_clock::now();

    heapSort(data);

    auto end = chrono::high_resolution_clock::now();
    // -------------------------------------------------------
    // END TIMER
    // -------------------------------------------------------

    chrono::duration<double> elapsed = end - start;
    double seconds = elapsed.count();

    // Print running time to console
    cout << "Running time: " << seconds << " seconds" << endl;

    // Write sorted output
    cout << "Writing sorted output to " << outputFile << "..." << endl;
    writeCSV(outputFile, data);

    // Also write running time into output file
    ofstream timeFile(outputFile, ios::app);
    timeFile << "\nRunning time: " << seconds << " seconds\n";
    timeFile.close();

    cout << "Done! Output saved to " << outputFile << endl;

    return 0;
}