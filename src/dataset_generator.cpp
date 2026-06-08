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
     unsigned long long seed = 24221324464ULL;
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


