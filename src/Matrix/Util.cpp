#include "../include/Matrix/Util.h"

#include <cmath>
#include <algorithm>
#include <vector>
#include <pthread.h>

float Util::ReLu(float num){
    return (num > 0) ? num : 0.0;
}

float Util::TanH(float num){
    if(num > 8)return 1.0;
    if(num < -8)return -1.0;
    float n1 = exp(num);
    float n2 = exp(-1*num);
    float ans = (n1 - n2)/(n1 + n2);
    return ans;
}

float Util::Sigmoid(float num){
    if(num < -10)return 0.0;
    return (1/(1+exp(-1*num)));
}

void Util::Softmax(std::vector<std::vector<float>>& vect){
    float denominator = 0.0;
    float maxval = vect[0][0];
    for(int i=0;i<vect.size();i++){
        for (int j=0;j<vect[i].size();j++) {
            if (vect[i][j] > maxval) {
                maxval = vect[i][j];
            }
        }
    }
    for(int i=0;i<vect.size();i++){
        for (int j=0;j<vect[i].size();j++) {
            vect[i][j] = exp(vect[i][j]-maxval);
            denominator += vect[i][j];
        }
    }
    int totalSize = vect.size()*vect[0].size();
    if (denominator == 0.0) {
        for(int i=0;i<vect.size();i++) {
            for (int j=0;j<vect[i].size();j++) {
                vect[i][j] = 1.0/totalSize;
            }
        }
    } else {
        for(int i=0;i<vect.size();i++) {
            for (int j=0;j<vect[i].size();j++) {
                vect[i][j] = vect[i][j] / denominator;
            }
        }
    }
}

std::vector<std::string> Util::split( std::string const& original, char separator ) {
    std::vector<std::string> results;
    std::string::const_iterator start = original.begin();
    std::string::const_iterator end = original.end();
    std::string::const_iterator next = std::find( start, end, separator );
    while ( next != end ) {
        results.push_back( std::string( start, next ) );
        start = next + 1;
        next = std::find( start, end, separator );
    }
    results.push_back( std::string( start, next ) );
    return results;
}

void Util::parallelizedMatrixTransVectorMult(float* a, float* bt, float* ct, 
int m, int n, int numOfThreads) {
    // on heap so it can be accessed by threads
    int* tid = new int[numOfThreads];
    
    pthread_t multThreads[numOfThreads];

    for (int i = 0; i < numOfThreads; i ++) {
        float** info = new float*[7];
        // float* fm = new float(m);
        // float* fn = new float(n);
        // float* fk = new float(k);

        tid[i] = i;

        info[0] = a;
        info[1] = bt;
        info[2] = ct;
        info[3] = new float(m);
        info[4] = new float(n);
        // tid[i]
        info[5] = new float(i);
        info[6] = new float(numOfThreads);
        
        pthread_create(&multThreads[i], NULL, Util::eachMatrixTransVectorMult, info);
    }

    for (int i = 0; i < numOfThreads; i ++) {
        pthread_join(multThreads[i], NULL);
    }

    delete [] tid;
}

void* Util::eachMatrixTransVectorMult(void* infoArray) {
    // get correct type of everything
    float** info = (float**) infoArray;
    float* a = info[0];
    float* bt = info[1];
    float* ct = info[2];
    int m = int(*info[3]);
    int n = int(*info[4]);
    int selfTID = int(*info[5]);
    int numOfThreads = int(*info[6]);

    // FIXME: assuming b and c given as transpose.
    for (int i=selfTID; i<m; i+=(numOfThreads)) {
        float eachDotProduct = 0.0;
        for (int j=0; j<n; j++) {
            eachDotProduct += (*(a+(i*n)+j)) * (*(bt+j));
        }
        *(ct+i) = eachDotProduct;
    }

    delete [] info;
    // nothing to return
    return nullptr;
}
