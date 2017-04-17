#ifndef PSOPT_H
#define PSOPT_H

#include <vector>
#include <stdlib.h>
#include <ctime>
#include <unistd.h>
#include <math.h>
#include <limits>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
using namespace std;

struct param{
    typedef pair<string, float> paramPair;
    paramPair popuSize;
    paramPair K;
    paramPair knn;
    paramPair maxIte;
    paramPair omega;
    paramPair alpha;
    paramPair beta;
    paramPair gamma;
    paramPair validate;
    vector<paramPair*> paramList;
    param() :
        popuSize(make_pair(string("popuSize"),0)),
        K(make_pair(string("K"),0)),
        knn(make_pair(string("knn"),0)),
        maxIte(make_pair(string("maxIte"),0)),
        omega(make_pair(string("omega"),0)),
        alpha(make_pair(string("alpha"),0)),
        beta(make_pair(string("beta"),0)),
        gamma(make_pair(string("gamma"),0)),
        validate(make_pair(string("validate"),0)) {
        paramList = {&popuSize, &K, &knn, &maxIte, &omega, &alpha, &beta, &gamma, &validate};
    }
};

template <typename T>
class psOpt
{
    struct individual{
        vector<T> W;
        vector<T> V;
        double fitness;
        double indiBest;
        vector<T> indiBestW;
        individual(): fitness(.0), indiBest(numeric_limits<T>::max()*-1) {}
        individual(individual* indiv): fitness(.0), indiBest(numeric_limits<T>::max()*-1),
            W(indiv->W), V(indiv->V) {}
    };
    typedef vector<individual*> population;

    vector<T> Z;
    unsigned int P, K, ite ,knn;
    float omega, alpha, beta, gamma;
    population popu;
    double popuBest;
    vector<T> popuBestW;
    float validation;

    void init();
    double fitnessEva(individual* indi);
    void groupBestW(individual* indi, int kNeighbour, vector<T>& bestW);
    void update(individual* indi);
    double validating(vector<T>& popuBestW_);

public:
    psOpt(const param& param_, const vector<T>& Z_);
    ~psOpt();

    void train();
    void saveResult();
    vector<vector<double> > records;
};

#endif // PSOPT_H
