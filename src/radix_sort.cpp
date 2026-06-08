


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





int main()
{

    vector<Record> dataset;
    string filename;

    cout<< " RADIX SORT PROGRAM " << endl;
    cout<< " Enter dataset filename:";
    cin >> filename;

    readCSV(filename, dataset);

    cout << " Records loaded:" << dataset.size()<< endl;

    return 0;
}
