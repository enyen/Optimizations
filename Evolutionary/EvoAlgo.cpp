#include "EvoAlgo.h"

template <typename T>
/**
 * @brief EvoAlgo<T>::EvoAlgo : initialise algo parameters
 * @param P_ : (in) population size
 * @param items_ : (in) gnome item
 * @param survived_ : (in) populatin survival rate
 * @param ite_ : (in) algo max iteration
 */
EvoAlgo<T>::EvoAlgo(unsigned int P_, const vector<T>& items_, double survived_, unsigned int ite_){
    P = P_;
    items = items_;
    ite = ite_;
    survived = (unsigned int)ceil(P*survived_);
    convergent = 0;
    popu.clear();
    parent.clear();
}

template <typename T>
/**
 * @brief EvoAlgo<T>::~EvoAlgo : delete all individual in heap
 */
EvoAlgo<T>::~EvoAlgo()
{
    parent.clear();
    for(auto indi : popu)
        delete indi;
    popu.clear();
}

template <typename T>
/**
 * @brief EvoAlgo<T>::init : initialise all individual in population with valid gnome
 */
void EvoAlgo<T>::init(){
    srand((unsigned)time(NULL));
    popu.clear();
    for(unsigned int i=0; i<P; i++){
        individual* indi = new individual();
        vector<T> items1 = items;
        vector<T> items2 = items;
        for(unsigned int j=0; j<items.size(); j++){
            unsigned int idx1 = rand()%items1.size();
            indi->indi.push_back(items1[idx1]);
            items1.erase(items1.begin()+idx1);
            unsigned int idx2 = rand()%items2.size();
            indi->indi.push_back(items2[idx2]);
            items2.erase(items2.begin()+idx2);
        }
        fitnessEva(indi);
        popu.push_back(indi);
    }
}

template <typename T>
/**
 * @brief EvoAlgo<T>::fitnessEva : evaluated fitness of individual
 * @param indi : (in) individual to be evaluated, (out) evaluated individual
 */
void EvoAlgo<T>::fitnessEva(individual* indi){
    indi->fitness = .0;
    // one way trip distance, maximised
    for(unsigned int i=0; i<(indi->indi.size()-1); i++){
        double dist = .0;
        for(unsigned int j=0; j<indi->indi[i].size(); j++)
            dist += pow(indi->indi[i][j]-indi->indi[i+1][j], 2);
        indi->fitness += sqrt(dist);
    }
}

template <typename T>
/**
 * @brief EvoAlgo<T>::externalSelect : select individual with good fitness
 */
void EvoAlgo<T>::externalSelect(){
    mergeSort(popu, 0, popu.size());
    // rank deterministic, according to survival rate
    for(unsigned int i=0; i<(P-survived); i++){
        delete popu[popu.size()-1];
        popu.pop_back();
    }

    // compute survived indi fitness mean
    survivedMean = .0;
    for(unsigned int i=0; i<survived; i++)
        survivedMean += popu[i]->fitness / survived;
}

template <typename T>
/**
 * @brief EvoAlgo<T>::parentSelect : select parent for reproduction
 */
void EvoAlgo<T>::parentSelect(){
    parent.clear();

    // uniform sample
//    for(unsigned int i=0; i<(P-survived); i++)
//        parent.push_back(popu[rand()%survived]);

    // roulette wheel - stochatic elitism
    double section = survivedMean/(P-survived)*survived;
    double cummulation = 0;
    double ball = (rand()%1000)*.001*section;
    for(unsigned int i=0; i<survived; i++){
        cummulation += popu[i]->fitness;
        while(ball < cummulation){
            ball += section;
            parent.push_back(popu[i]);
        }
    }
}

template <typename T>
/**
 * @brief EvoAlgo<T>::repopulate : produce new offspring base on parent individual
 */
void EvoAlgo<T>::repopulate(){
    // new indi = parent duplication + mutation
    for(auto child : parent){
        individual* indi = new individual(child);
        mutate(indi);
        fitnessEva(indi);
        popu.push_back(indi);
    }
    parent.clear();
}

template <typename T>
/**
 * @brief EvoAlgo<T>::mutate : mutate an individual
 * @param indi : (in) individual to be mutated, (out) mutated individual
 */
void EvoAlgo<T>::mutate(individual* indi){
    // n swap, depend on item size and convergent status
    unsigned int n = rand()%(unsigned int)(items.size()*.05 + convergent) + 1;
    for(unsigned int i=0; i<n; i++){
        unsigned int idx1 = rand()%items.size();
        unsigned int idx2 = rand()%items.size();
        T store = indi->indi[idx1];
        indi->indi[idx1] = indi->indi[idx2];
        indi->indi[idx2] = store;
    }
}

template <typename T>
/**
 * @brief EvoAlgo<T>::mergeSort : sort population according to individual's fitness
 * @param pop : (in) population to besorted, (out) sorted population
 * @param begin : (in) begin point for merge sort
 * @param end : (in) end point for merge sort
 */
void EvoAlgo<T>::mergeSort(population& pop, unsigned int begin, unsigned int end){
    if(end-begin<2) return;
    unsigned int middle = (end+begin)/2;
    mergeSort(pop, begin, middle);
    mergeSort(pop, middle, end);
    unsigned int leftPoint=begin ,rightPoint = middle;
    unsigned int leftSize = middle-begin;
    unsigned int rightSize = end-middle;
    population pop_(leftSize+rightSize);
    bool updown = true;
    for(int j=0; j<(leftSize+rightSize); j++){
        bool comp = false;
        if(updown)
            comp = pop[rightPoint]->fitness > pop[leftPoint]->fitness;
        else
            comp = pop[rightPoint]->fitness <= pop[leftPoint]->fitness;
        if(comp){
            pop_[j] = pop[rightPoint];
            if((rightPoint+1) < (middle+rightSize)) rightPoint+=1;
            else updown = false;
        }else{
            pop_[j] = pop[leftPoint];
            if((leftPoint+1) < (begin+leftSize)) leftPoint+=1;
            else updown = false;
        }
    }
    copy(pop_.begin(), pop_.end(), pop.begin()+begin);
}

template <typename T>
/**
 * @brief EvoAlgo<T>::showPopulation : cout display individual fitness and attributes
 * @param pop : (in) population to be displayed
 * @param best : (in) (true) display only the best individual, (false) display all individual in the population
 */
void EvoAlgo<T>::showPopulation(unsigned int idx, bool save){
    // refuses to cout non-interpretative coordinate, save & plot it instead!
    ofstream myfile;
    if(save)myfile.open("route.txt");
    cout << "fitness: " << popu[idx]->fitness << endl;
//    cout << "Attr: ";
    for(int j=0; j<popu[idx]->indi.size(); j++){
//        cout<<"(";
        for(int k=0; k<popu[idx]->indi[j].size(); k++){
//            cout << popu[idx]->indi[j][k] << ",";
            if(save) myfile << popu[idx]->indi[j][k] << " ";
        }
//        cout<<"\b"<<") ";
        if(save) myfile << "\b" << "\n";
    }
//    cout << endl;
    cout << endl;
    if(save) {
        myfile.close();
        cout<<"Route saved in [route.txt]"<<endl;
    }
}

template <typename T>
/**
 * @brief EvoAlgo<T>::train : Main EA routine cycle
 */
void EvoAlgo<T>::train(){
    cout<<"Training  0%  0%     0 "<<flush;
    init();
    records.clear();
    double best = -1.*numeric_limits<double>::max();
    unsigned int iteCount = 0, ConvergedCount = 0; bool flag=1;
    while((iteCount<ite) && (ConvergedCount<(ite/3))){
        iteCount++;
        externalSelect();
        //****** performance tracking ******//
        if(popu[0]->fitness > best){
            best = popu[0]->fitness;
            ConvergedCount=0;
        }else ConvergedCount++;
        convergent = (ConvergedCount>1)?log10(ConvergedCount):0;
        vector<double> performance;
        performance.push_back(survivedMean);
        performance.push_back(popu[0]->fitness);
        performance.push_back(popu[survived-1]->fitness);
        records.push_back(performance);
        if(flag) cout<<">"<<flush;
        else cout<<"\b<\b"<<flush;
        if(iteCount%25 == 0) flag=flag^1;
        if(iteCount%50 == 0) {
            cout<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b"<<setw(2)<<100*iteCount/ite<<"% ";
            cout<<setw(2)<<100*ConvergedCount/ite*3<<"% "<<setw(5)<<(int)best<<" "<<flush;
        }
        if(iteCount==ite){
            cout<<endl<<"No convergent. Extra N iteration (N/0) ? ";
            int ans; cin>>ans;
            ite += ans;
        }
        if(ConvergedCount==ite/3){
            cout<<endl<<"Converged. Extra N iteration (N/0) ? ";
            int ans; cin>>ans;
            ite += ans;
        }
        //**********************************//
        parentSelect();
        repopulate();
    }

    cout<<endl<<"After " <<iteCount<<" iterations ";
    if(iteCount == ite) cout << "(max";
    else cout<<"(converged "<< ite/3;
    cout<<" iterations), Best Idividual ";
    showPopulation(0,1);
}

template class EvoAlgo<vector<int> >;
template class EvoAlgo<vector<double> >;
template class EvoAlgo<vector<float> >;
