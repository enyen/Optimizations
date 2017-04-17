#ifndef EVOALGO_H
#define EVOALGO_H

#include <vector>
#include <stdlib.h>
#include <ctime>
#include <unistd.h>
#include <math.h>
#include <limits>
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

template <typename T>
class EvoAlgo
{
    struct individual{
        vector<T> indi;
        double fitness;
        individual(): fitness(.0) {}
        individual(individual* indiv): fitness(.0), indi(indiv->indi) {}
    };
    typedef vector<individual*> population;

    unsigned int P;
    vector<T> items;
    unsigned int ite;
    unsigned int survived;
    double survivedMean;
    double convergent;
    population popu;
    population parent;

    void init();
    void fitnessEva(individual* indi);
    void externalSelect();
    void parentSelect();
    void repopulate();
    void mutate(individual* indi);
    void mergeSort(population& pop, unsigned int begin, unsigned int end);

public:
    EvoAlgo(unsigned int P_, const vector<T>& items_, double survived_, unsigned int ite_);
    ~EvoAlgo();

    void train();
    void showPopulation(unsigned int idx, bool best=false);
    vector<vector<double> > records;
};

#endif // EVOALGO_H
