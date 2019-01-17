#include "Util.h"

#include <cmath>
#include <algorithm>
#include <vector>
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

std::vector<float> Util::Softmax(std::vector<float> vect){
    std::vector<float> ans;
    float maxval = vect[0];
    for(int i=1;i<vect.size();i++){
        if(maxval < vect[i])maxval = vect[i];
    }
    float denominator = 0.0;
    for(int i=0;i<vect.size();i++){
        ans.push_back(exp(vect[i]-maxval));
        denominator += ans[i];
    }
    for(int i=0;i<ans.size();i++)
        ans[i] = ans[i] / denominator;
    return ans;
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