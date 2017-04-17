#include "psOpt.h"
#include <string>

typedef double T;
void readFiles(string zFile, string paramFile, param& param_, vector<T>& z);
void savePerformanceFile(vector<vector<double> >& performance);

int main(int argc, const char* argv[]) {
    if (argc != 3) {
        cout << "usage: " << argv[0] << " <zFile> <paramFile>" << endl;
        exit(1);
    }

    param myParam; vector<T> z;
    readFiles(argv[1], argv[2], myParam, z);

    psOpt<T> myPSO(myParam, z);
    myPSO.train();
    savePerformanceFile(myPSO.records);

    return 0;
}

/**
 * @brief read_tz_File : read in t and z(t)
 * @param file : (in) input filename
 * @param points : (out) loaded positions
 */
void readFiles(string zfile, string paramfile, param& param_, vector<T>& z){
    fstream myfile;
    myfile.open(paramfile.c_str(), ios::in);
    if(!myfile.is_open()){
        cout<<"paramFile not found!"<<endl;
        exit(1);
    }
    string s; double d; float f;
    while(myfile >> s)
        for(int i=0; i<param_.paramList.size(); i++)
            if(!(s.compare(param_.paramList[i]->first))){
                myfile >> f;
                param_.paramList[i]->second = f;
                break;
            }
    myfile.close();
    cout<<"Parameters loaded"<<endl;

    myfile.open(zfile.c_str(), ios::in);
    if(!myfile.is_open()){
        cout<<"zFile not found!"<<endl;
        exit(1);
    }
    z.clear();
    getline(myfile,s);
    while(myfile >> d){
        myfile >> d;
        z.push_back(d);
    }
    myfile.close();
    cout<<"timeSeries loaded"<<endl;
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
