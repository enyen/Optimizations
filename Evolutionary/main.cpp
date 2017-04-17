#include "EvoAlgo.h"
#include <string>

typedef vector<int> T;
void readPositionFile(string filename, vector<T> & points);
void savePerformanceFile(vector<vector<double> >& performance);

int main(int argc, const char* argv[]) {
    if (argc != 5) {
        cout << "usage: " << argv[0] << " <locationFile> <populationSize> <survivalRate> <maxIteration>" << endl;
        exit(1);
    }

    vector<T> points;
    readPositionFile(argv[1], points);

    EvoAlgo<T> myEvo1(atoi(argv[2]), points, atof(argv[3]), atoi(argv[4]));
    myEvo1.train();
    savePerformanceFile(myEvo1.records);

    return 0;
}

/**
 * @brief readPositionFile : read in positions
 * @param file : (in) input filename
 * @param points : (out) loaded positions
 */
void readPositionFile(string file, vector<T> & points){
    fstream myfile;
    myfile.open(file, ios::in);
    if(!myfile.is_open()){
        cout<<"File not found!"<<endl;
        exit(1);
    }

    points.clear();
    string nonsense; myfile >> nonsense;
    int data; char c;
    while(myfile >> data){
        vector<int> thispoint;
        myfile >> data;
        thispoint.push_back(data);
        myfile >> data;
        thispoint.push_back(data);
        myfile >> c;
        points.push_back(thispoint);
    }
    cout<<endl<<"Position loaded"<<endl;
}

/**
 * @brief savePerformanceFile : save performances to file
 * @param performance : (in) [max, average, min] of parent pool
 */
void savePerformanceFile(vector<vector<double> >& performance){
    ofstream myfile("performance.txt");
    for(unsigned int i=0; i<performance.size(); i++){
        myfile << i << " ";
        for(unsigned int j=0; j<performance[i].size(); j++)
            myfile << performance[i][j] << " ";
        myfile << "\b" << "\n";
    }
    myfile.close();
    cout<<"Performances saved in [performance.txt]"<<endl<<endl;
}
