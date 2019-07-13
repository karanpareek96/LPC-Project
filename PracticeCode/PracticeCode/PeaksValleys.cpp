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

void printVec(std::vector<float> myVec){
    for (auto a: myVec){
        std::cout << a << " ";
    }
    std::cout << "\n";
}

void printArr(float *arr, size_t len){
    for (int i = 0; i < len; i++){
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
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
    
    computeParams(signal, signalLength);
    
}

PeaksAndValleys::~PeaksAndValleys(){

    delete height;
    delete spread;
    delete slope;
    delete peaks;
    delete valleys;
//    delete sigPointer;
    
}

void PeaksAndValleys::computeParams(float *signal, unsigned int signalLength){
    
    int i, j;
    
    // Initialize all values to -1
    float initValue[signalLength];
    for (i = 0; i < signalLength; i++){
        initValue[i] -= 1.0;
    }
    std::cout << "Init values: "; printArr(initValue, signalLength);
    
    int len = signalLength - 1;
    
    // Compute slopes array (has length = len)
    float slopes[len];
    for (i = 0; i < len; i++){
        slopes[i] = 0.5*(signal[i+1] - signal[i]);
    }
    std::cout << "Slope values: "; printArr(slopes, len);
    
    // The structs are set to -1
    for (i = 0; i < signalLength; i++){
        peaks->idx.push_back(initValue[i]);
        peaks->mag.push_back(initValue[i]);
        valleys->idx.push_back(initValue[i]);
        valleys->mag.push_back(initValue[i]);
    }
    
    std::cout << "Peaks idx: "; printVec(peaks->idx);
    std::cout << "Peaks mag: "; printVec(peaks->mag);
    std::cout << "Valleys idx: "; printVec(valleys->idx);
    std::cout << "Valleys mag: "; printVec(valleys->mag);
    
    // Find slopes changes
    if (slopes[0] > 0)
        slopePos(slopes);
    else
        slopeNeg(slopes);
    
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

    peaks->idx.resize(cntPeak);
    peaks->mag.resize(cntPeak);
    valleys->idx.resize(cntPeak+1);
    valleys->mag.resize(cntPeak+1);
    
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
    peaks->mag.at(cntPeak) = (*sigPointer)[k];
    
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
        //use calloc for memory allocation
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
