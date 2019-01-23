#ifndef UTIL_H
#define UTIL_H

// HELPER LIBRARY

#include <string>
#include <vector>

namespace Util
{
    float ReLu(float num);
    float TanH(float num);
    float Sigmoid(float num);
    void Softmax(std::vector<std::vector<float>>& vect);
    std::vector<std::string> split( std::string const& original, char separator );

    // c^T = a*b^T, a is mxn, b is nx1 vector, 
    void parallelizedMatrixTransVectorMult(float* a, float* bt, float* ct, 
        int m, int n, int numOfThreads);

    void* eachMatrixTransVectorMult(void* infoArray);
};

#endif /* UTIL_H */