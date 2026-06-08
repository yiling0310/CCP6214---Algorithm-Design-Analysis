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

     //group leader's id converted already mapped
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
 }
