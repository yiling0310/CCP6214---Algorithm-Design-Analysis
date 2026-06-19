
// *********************************************************
// Program: dataset_generator.cpp

// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC3L
// Tutorial Class: TT10L
// Trimester: 2610
// Member_1: 243UC247H3 | Schweeta a/p Kumaran| SCHWEETA.KUMARAN@student.edu.my | 016-207-2813
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



# include <iostream>
# include<fstream>
# include <vector>
# include <unordered_set>
# include <random>
# include <string>


using namespace std;

struct Record
{
     long long id;
     string word;
};

 // to generate random 5 letter lowercase string
 string stringGenerate(mt19937_64& randnumgen)
 {

     string s = "";
     uniform_int_distribution<int> distributeLetter(0,25);
     for(int i =0; i < 5; i++)
     {
         s+= char('a' + distributeLetter(randnumgen));

     }

     return s;

 }


 int main()
 {
     long long input;

     cout<< " Please enter the size of the dataset : ";

     cin>> input;

     //random number generator using group leader's seed
     unsigned long long seed = 2421324464ULL;
     mt19937_64 randnumgen(seed);

     // the range for the assignment is given as 1,000,000,000 to 9,999,999,999

     uniform_int_distribution<long long>
     distributeNum(1000000000LL , 9999999999LL);
     unordered_set<long long> usedNumbers;
     vector<Record> dataset;

     dataset.reserve(input);


     while (dataset.size() < input )
     {
         long long number= distributeNum(randnumgen);

         if(usedNumbers.find(number)== usedNumbers.end())
         {
             usedNumbers.insert(number);

             Record rec;

             rec.id= number;
             rec.word = stringGenerate(randnumgen);

             dataset.push_back(rec);
         }

 }


 //creating the csv file

 string filename =
 "dataset_" + to_string(input) + ".csv";

 ofstream file(filename);

 if(!file)
 {

     cout<< "Error creating the file !"<< endl;
     return 1;
 }

 for(size_t i=0; i< dataset.size(); i++)
 {
     file<< dataset[i].id<<","<< dataset[i].word<<"\n";
 }

 file.close();

 cout<< "\n The dataset has been successfully generated.\n";
 cout<< "   Output file: "<< filename << endl;
 cout<< "   Total records: "<< input << endl;

 return 0;

 }


