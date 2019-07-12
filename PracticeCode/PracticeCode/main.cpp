#include <iostream>
#include <vector>
#include "PeaksValleys.hpp"

int main(){
    
    float x[] {0.8, 0.64, -0.63};
    int len = sizeof(x)/sizeof(x[0]);
    
    PeaksAndValleys *test = new PeaksAndValleys(x, len);
    delete test;
    
    return 0;
}
