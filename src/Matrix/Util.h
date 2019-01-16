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
    std::vector<std::string> split( std::string const& original, char separator );
};

#endif /* UTIL_H */