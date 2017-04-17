#include "psOpt.h"

template <typename T>
/**
 * @brief psOpt<T>::psOpt : init algo
 * @param param_ [in]: input algo params
 * @param Z_ [in]: input data point
 */
psOpt<T>::psOpt(const param& param_, const vector<T>& Z_){
    srand((unsigned)time(NULL));
    Z = Z_;
    P = param_.popuSize.second;
    ite = param_.maxIte.second;
    knn = param_.knn.second;
    K = (Z.size()>=(param_.K.second+1)) ? param_.K.second : (Z.size()-1);
    omega = param_.omega.second;
    alpha = param_.alpha.second;
    beta = param_.beta.second;
    gamma = param_.gamma.second;
    popu.clear();
    popuBest = numeric_limits<T>::max()*-1;
    validation = param_.validate.second; // assume enough z size
}

template <typename T>
/**
 * @brief psOpt<T>::~psOpt : delete all individual in heap
 */
psOpt<T>::~psOpt()
{
    for(auto indi : popu)
        delete indi;
    popu.clear();
}

template <typename T>
/**
 * @brief psOpt<T>::init : initialise all individual in population with valid gnome
 */
void psOpt<T>::init(){
    popu.clear();
    for(unsigned int i=0; i<P; i++){
        individual* indi = new individual();
        indi->V.clear(); indi->W.clear();
        for(unsigned int j=0; j<K; j++){
            T norm = rand()*1.0/RAND_MAX;
            indi->W.push_back(norm);
            norm = rand()*1.0/RAND_MAX;
            indi->V.push_back(norm);
        }
        double newF = fitnessEva(indi);
        popu.push_back(indi);
        if(newF > popuBest){
            popuBest = newF;
            popuBestW = indi->W;
        }
    }
}

template <typename T>
/**
 * @brief psOpt<T>::fitnessEva : evaluated fitness of individual based on training data
 * @param indi : (in) individual to be evaluated, (out) evaluated individual
 */
double psOpt<T>::fitnessEva(individual* indi){
    double squerr = 0;
    for(int i=K; i<(int)(Z.size()*(1.f-validation)); i++){
        T y = 0.;
        for(int j=0; j<K; j++)
            y += indi->W[j] * Z[i-1.-j];
        squerr += pow(y-Z[i], 2);
    }
    indi->fitness = -1. * squerr / (Z.size()*(1.f-validation)-K);
    if(indi->fitness > indi->indiBest){
        indi->indiBest = indi->fitness;
        indi->indiBestW = indi->W;
    }
    return indi->fitness;
}

template <typename T>
/**
 * @brief psOpt<T>::validating : evaluated fitness of individual based on testing data
 * @param popuBestW_ : weights of an indivisual to be evaluated
 * @return
 */
double psOpt<T>::validating(vector<T>& popuBestW_){
    double squerr = 0;
    for(int i=(Z.size()*(1.f-validation)+K); i<Z.size(); i++){
        T y = 0.;
        for(int j=0; j<K; j++)
            y += popuBestW_[j] * Z[i-1.-j];
        squerr += pow(y-Z[i], 2);
    }
    squerr /= (Z.size()*validation-K);
    return -squerr;
}

template <typename T>
/**
 * @brief psOpt<T>::groupBestW : search for the best weights in local neighbourhood
 * @param indi [in] : center of the neighbourhood
 * @param kNeighbour [in] : radius of neighbourhood
 * @param bestW [out] : best weight in the neighbouthood
 */
void psOpt<T>::groupBestW(individual* indi, int kNeighbour, vector<T>& bestW){
    vector<double> dists(kNeighbour, numeric_limits<double>::max());
    vector<individual*> nnIndi(kNeighbour, NULL);
    for(int i=0; i<popu.size(); i++){
        double dist = 0;
        for(int j=0; j<K; j++)
            dist += pow(indi->W[i]-popu[i]->W[j],2);
        for(int j=0; j<kNeighbour; j++){
            if((dist < dists[j]) || (nnIndi[j] == NULL)){
                std::rotate(dists.begin()+j, dists.end()-1, dists.end());
                std::rotate(nnIndi.begin()+j, nnIndi.end()-1, nnIndi.end());
                nnIndi[j] = popu[i];
                dists[j] = dist;
                break;
            }
        }
    }
    double maxF=numeric_limits<double>::max()*-1.0;
    individual* bestNN = NULL;
    for(int i=0; i<kNeighbour; i++){
        if(nnIndi[i]->indiBest > maxF){
            maxF = nnIndi[i]->indiBest;
            bestNN = nnIndi[i];
        }
    }
    bestW = bestNN->W;
}

template <typename T>
/**
 * @brief psOpt<T>::update : update W and V of individual
 * @param indi [in] : indivisual to be updated
 */
void psOpt<T>::update(individual* indi){
    float randA=rand()*1./RAND_MAX, randB=rand()*1./RAND_MAX, randG=rand()*1./RAND_MAX;
    vector<T> groupW;
    groupBestW(indi, knn, groupW);
    for(int i=0; i<K; i++){
        indi->V[i] = omega*indi->V[i] + alpha*randA*(indi->indiBestW[i]-indi->W[i]) +
                beta*randB*(groupW[i]-indi->W[i]) + gamma*randG*(popuBestW[i]-indi->W[i]);
        indi->W[i] += indi->V[i];
    }
    fitnessEva(indi);
}

template <typename T>
/**
 * @brief psOpt<T>::saveResult : save the final weight in file
 */
void psOpt<T>::saveResult(){
    ofstream myfile("w.txt");
    for(int i=0; i<K; i++)
        myfile << popuBestW[i] << "\n";
    myfile.close();
    cout<<"W saved in [w.txt]"<<endl;

    myfile.open("result.txt");
    for(int i=K; i<Z.size(); i++){
        double y = 0.;
        for(int j=0; j<K; j++)
            y += popuBestW[j]*Z[i-1-j];
        myfile << i << " " << Z[i] << " " << y << endl;
    }
    myfile.close();
    cout<<"result saved in [result.txt]"<<endl;
}

template <typename T>
/**
 * @brief psOpt<T>::train : Main PSO routine cycle
 */
void psOpt<T>::train(){
    cout<<"Training  0%  0%        0 "<<flush;
    unsigned int iteCount = 0, ConvergedCount = 0; bool flag=1;
    records.clear();

    init();
    while((iteCount<ite) && (ConvergedCount<(ite/3))){
        iteCount++;
        for(int i=0; i<popu.size(); i++)
            update(popu[i]);
        for(int i=0; i<popu.size(); i++)
            if(popu[i]->indiBest > popuBest){
                popuBest = popu[i]->indiBest;
                popuBestW = popu[i]->indiBestW;
                ConvergedCount=0;
            }
        ConvergedCount++;
        //****** performance tracking ******//
        vector<double> performance;
        performance.push_back(popuBest);
        performance.push_back(validating(popuBestW));
        records.push_back(performance);
        if(flag) cout<<">"<<flush;
        else cout<<"\b<\b"<<flush;
        if(iteCount%25 == 0) flag=flag^1;
        if(iteCount%50 == 0) {
            cout<<"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"<<setw(2)<<100*iteCount/ite<<"% ";
            cout<<setw(2)<<100*ConvergedCount/ite*3<<"% "<<setw(8)<<setprecision(5)<<fixed<<popuBest<<" "<<flush;
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
    }

    cout<<endl<<"After " <<iteCount<<" iterations ";
    if(iteCount == ite) cout << "(max";
    else cout<<"(converged "<< ConvergedCount;
    cout<<" iterations), Best Idividual "<<popuBest<<endl;
    cout<<"Validation Fitness: "<<validating(popuBestW)<<endl;
    saveResult();
}

template class psOpt<double>;
template class psOpt<float>;
