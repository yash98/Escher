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
    std::vector<float> Softmax(std::vector<float> vect);
};

#endif /* UTIL_H */