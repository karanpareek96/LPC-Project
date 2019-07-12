//
//  PeaksValleys.cpp
//  PracticeCode
//
//  Created by Karan Pareek on 11/07/19.
//  Copyright © 2019 Karan Pareek. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cmath>
#include "PeaksValleys.hpp"

/*
 Helper functions
 */

template <typename T>
T meanValue(T a, T b){
    return T (a+b)/2;
}

int sign(float v){
    if (v > 0.0)
        return 1;
    else if (v < 0.0)
        return -1;
    else
        return 0;
}

/*
 Class member functions
 */

PeaksAndValleys::PeaksAndValleys(float *signal, unsigned int signalLength){
    
    valleyPair      = 0;
    cntValley       = 1;
    cntPeak         = 1;
    prevSlopePos    = 1;
    init            = false;
    
    height  = new HEIGHT;
    spread  = new SPREAD;
    slope   = new SLOPE;
    peaks   = new PEAKS;
    valleys = new VALLEYS;
    
    sigPointer = &signal;
    
    std::cout << "Create constructor" << std::endl;
    
    computeParams(signal, signalLength);
    
}

PeaksAndValleys::~PeaksAndValleys(){

    delete height;
    delete spread;
    delete slope;
    delete peaks;
    delete valleys;
    delete sigPointer;
    
}

void PeaksAndValleys::computeParams(float *signal, unsigned int signalLength){
    
    int i, j;
    
    // Initialization values
    float slopes[signalLength];
    for (i = 0; i < signalLength; i++){
        slopes[i] -= 1.0;
    }
    int len = signalLength - 1;
    
    std::cout << "Len: " << len << std::endl;
    
    // Compute slopes
    for (i = 0; i < len; i++){
        slopes[i] = 0.5*(slopes[i+1] - slopes[i]);
    }
    
    std::cout << "Slope stuff" << std::endl;
    
    for (i = 0; i < signalLength; i++){
        peaks->idx.push_back(slopes[i]);
        peaks->mag.push_back(slopes[i]);
        valleys->idx.push_back(slopes[i]);
        valleys->mag.push_back(slopes[i]);
    }
    
    std::cout << "Vec assignment" << std::endl;
    
    // Find slopes changes
    if (slopes[0] > 0)
        slopePos(slopes);
    else
        slopeNeg(slopes);
    
    std::cout << "Pos/neg stuff" << std::endl;
    
    if (!prevSlopePos){
        for (j = 0; j < len; i++){
            if (valleys->idx.at(j) < 0){
                valleys->idx.at(j) = len + 1;
                valleys->mag.at(j) = signal[signalLength];
                break;
            }
        }
    }
    
    cntPeak = cntPeak - 1;
    
    /*
     peaks.idx   = peaks.idx(1:cntPeak);
     peaks.mag   = peaks.mag(1:cntPeak);
     
     valleys.idx = valleys.idx(1:(cntPeak+1));
     valleys.mag = valleys.mag(1:(cntPeak+1));
     */
    
    analysis();
    
}

void PeaksAndValleys::slopeNeg(float *slopes){
    
    int i;
    
    for (i = 0; i < len; i++){
        if (sign(slopes[i]) < 0){
            if (!init)
                continue;
            if (prevSlopePos){
                updatePeaks(i);
                prevSlopePos = 0.0;
            }
        }
        else{
            if (!init){
                updateValleys(i);
                init = true;
                continue;
            }
            if (!prevSlopePos)
                updateValleys(i);
            
            prevSlopePos = 1;
        }
    }
}

void PeaksAndValleys::slopePos(float *slopes){
    
    int i;
    
    for (i = 0; i < len; i++){
        if (sign(slopes[i]) < 0){
            if (!init){
                init = true;
                prevSlopePos = 0.0;
                continue;
            }
            if (prevSlopePos){
                updatePeaks(i);
                prevSlopePos = 0.0;
            }
        }
        else{
            if (!init){
                updateValleys(i);
                init = true;
                continue;
            }
            if (!prevSlopePos)
                updateValleys(i);
            
            prevSlopePos = 1;
        }
    }
}

void PeaksAndValleys::updatePeaks(int k){
    
    peaks->idx.at(cntPeak) = k;
    peaks->mag.at(cntPeak) = *sigPointer[k];
    
    cntPeak = cntPeak + 1;
}

void PeaksAndValleys::updateValleys(int k){
    int rangeLow, rangeHigh;
    
    if (valleyPair == 0){
        rangeLow = cntValley;
        rangeHigh = cntValley + 1;
        
        valleys->idx.at(rangeLow)  = k;
        valleys->idx.at(rangeHigh) = -1;
        
        valleys->mag.at(rangeLow)  = *sigPointer[k];
        valleys->mag.at(rangeHigh) = -1;
    }
    else if (valleyPair == 1){
        rangeLow  = cntValley;
        rangeHigh = cntValley + 1;
        
        valleys->idx.at(rangeLow)  = valleys->idx.at(cntValley);
        valleys->idx.at(rangeHigh) = k;

        valleys->mag.at(rangeLow)  = valleys->mag.at(cntValley);
        valleys->mag.at(rangeHigh) = *sigPointer[k];
        
        valleyPair = valleyPair + 1;
    }
    else if (valleyPair == 2){
        valleyPair = 1;
        cntValley = cntValley + 2;
        
        rangeLow  = cntValley;
        rangeHigh = cntValley + 1;
        
        valleys->idx.at(rangeLow)  = k;
        valleys->idx.at(rangeHigh) = -1;
        
        valleys->mag.at(rangeLow)  = *sigPointer[k];
        valleys->mag.at(rangeHigh) = -1;
    }
}

void PeaksAndValleys::analysis(){
    
    int i, k;
    float diffL, diffR, slopeL, slopeR;
    
    // Need to access the height paramater from peaks
    size_t len1 = peaks->mag.size();
    for (i = 0; i < len1; i++){
//        use calloc for memory allocation
        peaks->height.left.push_back(0.0);
        peaks->height.right.push_back(0.0);
    }
    
    for (k = 0; k < len1; k++){
        diffL = std::abs(peaks->mag.at(k) - valleys->mag.at(k));
        diffR = std::abs(peaks->mag.at(k) - valleys->mag.at(k+1));
        
        peaks->height.left.at(k)  = std::max(diffL, diffR);
        peaks->height.right.at(k) = meanValue(diffL, diffR);

        
        slopeL = diffL / (peaks->idx.at(k) - valleys->idx.at(k));
        slopeR = diffR / (valleys->idx.at(k+1) - peaks->idx.at(k));
        
        peaks->slope.left.at(k)  = std::max(slopeL, slopeR);
        peaks->slope.right.at(k) = meanValue(slopeL, slopeR);
        
        peaks->spread.left.at(k)  = peaks->idx.at(k) - valleys->idx.at(k);
        peaks->spread.right.at(k) = valleys->idx.at(k+1) - peaks->idx.at(k);
    }
}
