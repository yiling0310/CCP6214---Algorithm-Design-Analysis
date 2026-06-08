


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
