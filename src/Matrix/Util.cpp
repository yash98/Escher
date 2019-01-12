#include "Util.h"

#include <cmath>


float Util::ReLu(float num){
    return (num > 0) ? num : 0.0;
}

float Util::TanH(float num){
    float n1 = exp(num);
    float n2 = exp(-1*num);
    float ans = (n1 - n2)/(n1 + n2);
    return ans;
}

float Util::Sigmoid(float num){
    return (1/(1+exp(-1*num)));
}

std::vector<float> Util::Softmax(std::vector<float> vect){
    std::vector<float> ans;
    ans.reserve(vect.size());
    float denominator = 0;
    for(int i=0;i<vect.size();i++){
        ans[i] = exp(vect[i]);
        denominator += ans[i];
    }
    for(int i=0;i<ans.size();i++)
        ans[i] /= denominator;
    return ans;
}