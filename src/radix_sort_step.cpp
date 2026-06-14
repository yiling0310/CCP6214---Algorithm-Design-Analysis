

// *********************************************************
// Program: radix_sort_step.cpp

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

using namespace std;

struct Record
{
    long long id;
    string word;
};



// declare the necessary functions

 void readCSV(string filename, vector<Record>& dataset);
 void countingSort(vector<Record>& array, long long exponent);
 void radixSort(vector<Record>&array,ofstream& stepFile,int startRow, int endRow);
 void writeSortStep(vector<Record>& array, ofstream& stepFile,string label, int startRow, int endRow);

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

void writeSortStep(vector<Record>& array, ofstream& stepFile, string label, int startRow, int endRow)

{


    stepFile<<"[";

    for(int i= startRow-1; i<= endRow-1 && i < array.size(); i++)
    {
        stepFile<<array[i].id<<"/"<< array[i].word;

        if(i != endRow-1 && i<array.size()-1)
        {
            stepFile<<", ";
        }
    }

    stepFile << "]" << label << "\n" ;

}





    void radixSort(vector<Record>&array, ofstream& stepFile, int startRow,int endRow)
    {
        if(array.empty())
        {
            return;
        }

        writeSortStep(array,stepFile,"original", startRow, endRow);

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

    int totalDigits = 0;
    long long temp = maximum;


    while(temp>0)

    {
        totalDigits++;
        temp/=10;
    }

    int currentDigit= totalDigits;


        // perform the radix sort
        for (long long exponent=1;maximum/exponent>0; exponent *=10)
        {
            countingSort(array, exponent);

            writeSortStep(array,stepFile,"d="+ to_string(currentDigit),startRow, endRow);

            currentDigit--;

        }

    }



int main()
{

    vector<Record> dataset;
    string filename;

    int startRow;
    int endRow;

    cout<< " RADIX SORT STEP PROGRAM " << endl;

    cout<< " Enter dataset filename: ";
    cin >> filename;

    cout<<"Enter the start row:" ;
    cin>>startRow;

    cout<<"Enter end row:";
    cin>>endRow;

    //to read the dataset
    readCSV(filename, dataset);


    if (dataset.empty())
    {
        cout<<"Dataset is empty !"<<endl;

        return 1;
    }

    //check the start row
    if(startRow<1)
    {
        cout<<"Invalid start row!" << endl;

        return 1;
    }

    //check the end row
        if(endRow > dataset.size())
       {
           endRow=dataset.size();
       }

       // check the range of the row
       if(startRow > endRow)
       {
           cout << "Invalid row range !" << endl;

           return 1;
       }


    //create output filename
    string outputFilename= filename.substr(0, filename.find(".csv"));
    outputFilename +="_radix_sorted_step_" + to_string(startRow)+ "_" + to_string(endRow)+ ".txt";

    ofstream stepFile(outputFilename);

    if(!stepFile)
    {
        cout<<"Error creating the output file!" <<endl;

        return 1;
    }



    radixSort(dataset, stepFile, startRow, endRow);

    stepFile.close();

    cout<< endl;

    cout<<"Step file successfully created !" << endl;
    cout<<"Output file:" << outputFilename<< endl;

    return 0;

}
