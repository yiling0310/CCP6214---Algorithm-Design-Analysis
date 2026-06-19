
// *********************************************************
// Program: radix_sort.cpp

// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC3L
// Tutorial Class: TT10L
// Trimester: 2610
// Member_1: 243UC247H3 | Schweeta a/p Kumaran| SCHWEETA.KUMARAN@student.edu.my | 016-207-2813
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


# include <iostream>
# include <fstream>
# include <vector>
# include <string>
# include <sstream>
# include <chrono> //for the time

using namespace std;

struct Record
{
    long long id;
    string word;
};


//the functions
void readCSV(string filename, vector<Record>& dataset)
{

    ifstream file(filename);

    if(!file)
    {
        cout<< " Error opening the file!"<< endl;
        return;
    }

    string line;

    while(getline(file,line))
    {

        stringstream ss(line);
        string idString;
        string word;


        getline(ss, idString, ',');
        getline(ss, word);

        Record rec;

        rec.id = stoll(idString);
        rec.word = word;

        dataset.push_back(rec);
    }

    file.close();
}


void countingSort(vector<Record>& array, long long exponent)
{

    int n = array.size();

    vector<Record>output(n);

    int count[10]={0};

    //count the occurrences of the digits

    for(int i =0; i<n; i++)
    {
        int digit= (array[i].id/ exponent)% 10; //take the digit at the current place value

        count[digit]++;
    }

    //converting the count array to positions
    for(int i=1; i < 10; i++)
    {
        count[i]+= count[i-1];
    }

    //build the output array
    for(int i= n-1; i >=0; i--)
    {
        int digit= (array[i].id / exponent) % 10;

        output[count[digit]-1]=array[i];

        count[digit]--;
    }

    for(int i =0; i<n; i++)
    {
        array[i]=output[i];
    }


}


    void radixSort(vector<Record>&array)
    {

     //take the records and sort it

    long long maximum = array[0].id; //assume the first number is the largest for now

    //check every element
    for(size_t i=1; i<array.size(); i++)
    {
        if(array[i].id > maximum)
        {
            maximum= array[i].id;
        }
    }

        for (long long exponent=1;maximum/exponent>0; exponent *=10)
        {
            countingSort(array, exponent);
        }

    }


    // save the sorted dataset

    void saveCSV( string filename, vector<Record>&dataset,double runtime)
    {
        ofstream file(filename);

        if(!file)
        {
            cout<< "Error creating the file !" << endl;
            return;
        }

        for(size_t i=0; i< dataset.size(); i++)
        {
            file<<dataset[i].id<<","<< dataset[i].word<<"\n";
        }


        file<<"\nRunning time:" << runtime<<"seconds\n";

        file.close();
    }



int main(int argc, char* argv[])
{

    vector<Record> dataset;

    if(argc !=2)
    {
        cout<<"Usage:"<<argv[0]<<"<dataset_file>"<<endl;

        return 1;
    }

    string filename = argv[1];

    //to read the dataset
    readCSV(filename, dataset);

    if(dataset.empty())
    {
         cout << "Dataset is empty ! "<< endl;
         return 1;
    }


    cout << " Records loaded:" << dataset.size()<< endl;

    // start the timing
    auto start= chrono::high_resolution_clock::now();

    radixSort(dataset);

    //stop the timing
    auto end= chrono:: high_resolution_clock::now();

    //calculate the running time
    double runtime= chrono:: duration<double>(end-start).count();

    cout<< " Dataset has been successfully sorted !" << endl;


    //save the sorted data
    saveCSV("radix_sorted_" + filename, dataset,runtime);

    cout<< "The data has been sorted !" <<endl;
    cout<< "Output file :" << "radix_sorted_" + filename<<endl;

    //display the running time
    cout<<"Running time : " << runtime<<" seconds "<<endl;

    return 0;
}
