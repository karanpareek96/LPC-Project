//
//  PeaksValleys.cpp
//  PracticeCode
//
//  Created by Karan Pareek on 11/07/19.
//  Copyright © 2019 Karan Pareek. All rights reserved.
//

// GIT TEST

#include <iostream>
#include <vector>
#include <cmath>
#include "PeaksAndValleys.hpp"

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

void printArr(float *arr, int len){
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
    
    peaks   = new PEAKS;
    valleys = new VALLEYS;
//    height  = new HEIGHT;
//    spread  = new SPREAD;
//    slope   = new SLOPE;

    sigPointer = signal;
    
    computeParams(signal, signalLength);
}

PeaksAndValleys::~PeaksAndValleys(){
    
//    delete height;
//    delete spread;
//    delete slope;
    delete peaks;
    delete valleys;
    
    free(initValue);
    free(slopes);
    //    delete sigPointer;
}

void PeaksAndValleys::computeParams(float *signal, unsigned int signalLength){
    
    int i, j;
    
    // Initialize all values to -1
    //    float initValue[signalLength];
    
    // use calloc to assign memory
    initValue = (float*) calloc(signalLength, sizeof(float));
    for (i = 0; i < signalLength; i++){
        initValue[i] -= 1.0;
    }
    std::cout << "Init values: "; printArr(initValue, signalLength);
    
    len = signalLength - 1;
    
    // Compute slopes array (has length = len)
    //    float slopes[len];
    slopes = (float*) malloc(len * sizeof(float));
    for (i = 0; i < len; i++){
        slopes[i] = 0.5*(signal[i+1] - signal[i]);
    }
    std::cout << "Slope values: "; printArr(slopes, len);
    
    // The struct values are set to -1 (slopes)
    peaks->idx = (float*) malloc(len * sizeof(float));
    peaks->mag = (float*) malloc(len * sizeof(float));
    valleys->idx = (float*) malloc(len * sizeof(float));
    valleys->mag = (float*) malloc(len * sizeof(float));
    
    // copy the values from slopes to each of them
    memcpy(peaks->idx, slopes, len * sizeof(float));
    memcpy(peaks->mag, slopes, len * sizeof(float));
    memcpy(valleys->idx, slopes, len * sizeof(float));
    memcpy(valleys->mag, slopes, len * sizeof(float));
    
    std::cout << "Peaks idx: "; printArr(peaks->idx, len);
    std::cout << "Peaks mag: "; printArr(peaks->mag, len);
    std::cout << "Valleys idx: "; printArr(valleys->idx, len);
    std::cout << "Valleys mag: "; printArr(valleys->mag, len);
    
    // Find slopes changes
    if (slopes[0] > 0)
        slopePos(slopes);
    else
        slopeNeg(slopes);
    
    std::cout << "-----------------------------\n";
    std::cout << "Peaks idx: "; printArr(peaks->idx, len);
    std::cout << "Peaks mag: "; printArr(peaks->mag, len);
    std::cout << "Valleys idx: "; printArr(valleys->idx, len);
    std::cout << "Valleys mag: "; printArr(valleys->mag, len);
    
    if (!prevSlopePos){
        for (j = 0; j < len; i++){
            if (valleys->idx[j] < 0){
                valleys->idx[j] = len + 1;
                valleys->mag[j] = signal[signalLength];
                break;
            }
        }
    }
    
    cntPeak = cntPeak - 1;
    
    // NOTE: Take only a certain part of the array
//    peaks.idx   = peaks.idx(1:cntPeak);
//    peaks.mag   = peaks.mag(1:cntPeak);
//
//    valleys.idx = valleys.idx(1:(cntPeak+1));
//    valleys.mag = valleys.mag(1:(cntPeak+1));
    
    std::cout << "-----------------------------\n";
    std::cout << "Peaks idx: "; printArr(peaks->idx, len);
    std::cout << "Peaks mag: "; printArr(peaks->mag, len);
    std::cout << "Valleys idx: "; printArr(valleys->idx, len);
    std::cout << "Valleys mag: "; printArr(valleys->mag, len);
    
    std::cout << "CntPeak value: " << cntPeak << std::endl;
    
    analysis(cntPeak);
}

void PeaksAndValleys::slopeNeg(float *slopes){
    
    int i;
    
    for (i = 0; i < len; i++){
        if (sign(slopes[i]) < 0){
            if (!init)
                continue;
            if (prevSlopePos){
                updatePeaks(i);
                prevSlopePos = 0;
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
    std::cout << "len: " << len << "\n";
    for (i = 0; i < len; i++){
        std::cout << "i: " << i << "\n";
        std::cout << "slopes[i]: " << slopes[i] << "\n";
        if (sign(slopes[i]) < 0){
            std::cout <<"init: " << init << "\n";
            std::cout <<"prevSlopePos: " << prevSlopePos << "\n";
            if (!init){
                init = true;
                prevSlopePos = 0;
                continue;
            }
            if (prevSlopePos){
                updatePeaks(i);
                prevSlopePos = 0;
            }
        }
        else{
            std::cout <<"init: " << init << "\n";
            std::cout <<"prevSlopePos: " << prevSlopePos << "\n";
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
    
    std::cout << "k: " << k << "\n";
    std::cout << "cntPeak: " << cntPeak << "\n";
    
    peaks->idx[cntPeak] = k;
    peaks->mag[cntPeak] = sigPointer[k];
    
    cntPeak = cntPeak + 1;
}

void PeaksAndValleys::updateValleys(int k){
    int rangeLow, rangeHigh;
    
    if (valleyPair == 0){
        rangeLow = cntValley;
        rangeHigh = cntValley + 1;

        valleys->idx[rangeLow]  = k;
        valleys->idx[rangeHigh] = -1;
        
        std::cout<<"*sigPointer[k]: "<<sigPointer[k]<<"\n";
        valleys->mag[rangeLow]  = sigPointer[k];
        valleys->mag[rangeHigh] = -1;
    }
    else if (valleyPair == 1){
        rangeLow  = cntValley;
        rangeHigh = cntValley + 1;
        
        valleys->idx[rangeLow]  = valleys->idx[cntValley];
        valleys->idx[rangeHigh] = k;
        
        valleys->mag[rangeLow]  = valleys->mag[cntValley];
        valleys->mag[rangeHigh] = sigPointer[k];
        
        valleyPair = valleyPair + 1;
    }
    else if (valleyPair == 2){
        valleyPair = 1;
        cntValley = cntValley + 2;
        
        rangeLow  = cntValley;
        rangeHigh = cntValley + 1;
        
        valleys->idx[rangeLow]  = k;
        valleys->idx[rangeHigh] = -1;
        
        valleys->mag[rangeLow]  = sigPointer[k];
        valleys->mag[rangeHigh] = -1;
    }
}

void PeaksAndValleys::analysis(unsigned int cntPeak){
    
    int k;
    float diffL, diffR, slopeL, slopeR;
    
    // set values to 0
    peaks->height.left = (float*) malloc(cntPeak * sizeof(float));
    peaks->height.right = (float*) malloc(cntPeak * sizeof(float));
    
    peaks->slope.left = (float*) malloc(cntPeak * sizeof(float));
    peaks->slope.right = (float*) malloc(cntPeak * sizeof(float));
    
    peaks->spread.left = (float*) malloc(cntPeak * sizeof(float));
    peaks->spread.right = (float*) malloc(cntPeak * sizeof(float));
    
    for (k = 0; k < cntPeak; k++){
        
        diffL = std::abs(peaks->mag[k] - valleys->mag[k]);
        diffR = std::abs(peaks->mag[k] - valleys->mag[k+1]);
        
        peaks->height.left[k]  = std::max(diffL, diffR);
        peaks->height.right[k] = meanValue(diffL, diffR);
        
        slopeL = diffL / (peaks->idx[k] - valleys->idx[k]);
        slopeR = diffR / (valleys->idx[k+1] - peaks->idx[k]);
        
        peaks->slope.left[k]  = std::max(slopeL, slopeR);
        peaks->slope.right[k] = meanValue(slopeL, slopeR);
        
        peaks->spread.left[k]  = peaks->idx[k] - valleys->idx[k];
        peaks->spread.right[k] = valleys->idx[k+1] - peaks->idx[k];
        
        std::cout << "-----------------------------\n";
        std::cout << "Peaks idx: "; printArr(peaks->idx, len);
        std::cout << "Peaks mag: "; printArr(peaks->mag, len);
        std::cout << "Valleys idx: "; printArr(valleys->idx, len);
        std::cout << "Valleys mag: "; printArr(valleys->mag, len);
    }
}
