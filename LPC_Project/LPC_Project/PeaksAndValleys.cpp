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

void printFloatArr(float *arr, int len){
    for (int i = 0; i < len; i++){
        std::cout << arr[i] << ", ";
    }
    std::cout << "\n";
}

void printIntArr(int *arr, int len){
    for (int i = 0; i < len; i++){
        std::cout << arr[i] << ", ";
    }
    std::cout << "\n";
}

/*
 Class member functions
 */

PeaksAndValleys::PeaksAndValleys(float *signal, unsigned int signalLength){
    
    valleyPair      = 0;
    cntValley       = 0;
    cntPeak         = 0;
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
    len = signalLength - 1;
    
    // Initialize all values to -1
    initValue = (float*) calloc(signalLength, sizeof(float));
    for (i = 0; i < signalLength; i++){
        initValue[i] = -1.0;
    }
    
    slopes = (float*) calloc(len, sizeof(float));
    for (i = 0; i < len; i++){
        slopes[i] = -1.0;
    }
    
    peaks->idx   = new int[signalLength];
    peaks->mag   = new float[signalLength];
    memset(peaks->idx, -1, signalLength * sizeof(int));
    memcpy(peaks->mag, initValue, signalLength * sizeof(float));
    
    valleys->idx   = new int[signalLength];
    valleys->mag   = new float[signalLength];
    memset(valleys->idx, -1, signalLength * sizeof(int));
    memcpy(valleys->mag, initValue, signalLength * sizeof(float));
    
    std::cout << "\nInitialized Values: \n";
    std::cout << "Peaks idx:   "; printIntArr(peaks->idx, signalLength);
    std::cout << "Peaks mag:   "; printFloatArr(peaks->mag, signalLength);
    std::cout << "Valleys idx: "; printIntArr(valleys->idx, signalLength);
    std::cout << "Valleys mag: "; printFloatArr(valleys->mag, signalLength);
    std::cout << "-----------------------------\n";
    
    // Compute slopes array
    for (i = 0; i < len; i++){
        slopes[i] = 0.5*(signal[i+1] - signal[i]);
    }
    std::cout << "Slope values: "; printFloatArr(slopes, len);
    std::cout << "-----------------------------\n";
    
    // Find slopes changes
    if (slopes[0] > 0)
        slopePos();
    else
        slopeNeg();
    
    if (!prevSlopePos){
        for (j = 0; j < len; i++){
            if (valleys->idx[j] < 0){
                valleys->idx[j] = len + 1;
                valleys->mag[j] = signal[signalLength];
                break;
            }
        }
    }
    
//    cntPeak = cntPeak - 1;
    
    std::cout << "Peaks idx:   "; printIntArr(peaks->idx, cntPeak);
    std::cout << "Peaks mag:   "; printFloatArr(peaks->mag, cntPeak);
    std::cout << "Valleys idx: "; printIntArr(valleys->idx, cntPeak + 1);
    std::cout << "Valleys mag: "; printFloatArr(valleys->mag, cntPeak + 1);
    
    std::cout << "cntPeak value:   " << cntPeak << std::endl;
    std::cout << "cntValley value: " << cntValley << std::endl;
    std::cout << "-----------------------------\n";
    
    /*
     
     What to do with truncating array?
     ie.
         peaks.idx   = peaks.idx(1:cntPeak);
         peaks.mag   = peaks.mag(1:cntPeak);
     
         valleys.idx = valleys.idx(1:(cntPeak+1));
         valleys.mag = valleys.mag(1:(cntPeak+1));
     
     Create new array of only cntPeak for Peaks and cntPeak+1 for Valleys?
     
     */
    
    // Make a temp array rather than final
    peaks->idx_final   = new int[cntPeak];
    peaks->mag_final   = new float[cntPeak];
    memcpy(peaks->idx_final, peaks->idx, cntPeak * sizeof(int));
    memcpy(peaks->mag_final, peaks->mag, cntPeak * sizeof(float));
    
    valleys->idx_final = new int[cntPeak + 1];
    valleys->mag_final = new float[cntPeak + 1];
    memcpy(valleys->idx_final, valleys->idx, (cntPeak+1) * sizeof(int));
    memcpy(valleys->mag_final, valleys->mag, (cntPeak+1) * sizeof(float));
    
    std::cout << "Final Arrays\n";
    std::cout << "Peaks idx_final:   "; printIntArr(peaks->idx_final, cntPeak);
    std::cout << "Peaks mag_final:   "; printFloatArr(peaks->mag_final, cntPeak);
    std::cout << "Valleys idx_final: "; printIntArr(valleys->idx_final, cntPeak + 1);
    std::cout << "Valleys mag_final: "; printFloatArr(valleys->mag_final, cntPeak + 1);
    std::cout << "-----------------------------\n";
    
    analysis();
}

void PeaksAndValleys::slopeNeg(){
    
    for (int i = 0; i < len; i++){
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

void PeaksAndValleys::slopePos(){
    
    for (int i = 0; i < len; i++){
        if (sign(slopes[i]) < 0){
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

    peaks->idx[cntPeak] = k;
    peaks->mag[cntPeak] = sigPointer[k];
    
    cntPeak = cntPeak + 1;
}

void PeaksAndValleys::updateValleys(int k){
    int rangeLow, rangeHigh;
    
    if (valleyPair == 0){
        rangeLow  = cntValley;
        rangeHigh = cntValley + 1;

        valleys->idx[rangeLow]  = k;
        valleys->idx[rangeHigh] = -1;
        
        valleys->mag[rangeLow]  = sigPointer[k];
        valleys->mag[rangeHigh] = -1;
        
        valleyPair = valleyPair + 1;
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
        cntValley  = cntValley + 2;
        
        rangeLow  = cntValley;
        rangeHigh = cntValley + 1;
        
        valleys->idx[rangeLow]  = k;
        valleys->idx[rangeHigh] = -1;
        
        valleys->mag[rangeLow]  = sigPointer[k];
        valleys->mag[rangeHigh] = -1;
    }
}

void PeaksAndValleys::analysis(){
    
    int k;
    float diffL, diffR, slopeL, slopeR;
    
    // set values to 0
    
    for (k = 0; k < cntPeak; k++){
        /*
         
         Decide what to do with this, whether to change these to mag_final or leave as is
         and make the temp arrays
         
        */
        
//        diffL = std::abs(peaks->mag[k] - valleys->mag[k]);
//        diffR = std::abs(peaks->mag[k] - valleys->mag[k+1]);
        
        diffL = std::abs(peaks->mag_final[k] - valleys->mag_final[k]);
        diffR = std::abs(peaks->mag_final[k] - valleys->mag_final[k+1]);
        
        peaks->height.left[k]  = std::max(diffL, diffR);
        peaks->height.right[k] = meanValue(diffL, diffR);
        
//        slopeL = diffL / (peaks->idx[k] - valleys->idx[k]);
//        slopeR = diffR / (valleys->idx[k+1] - peaks->idx[k]);
        
        slopeL = diffL / (peaks->idx_final[k] - valleys->idx_final[k]);
        slopeR = diffR / (valleys->idx_final[k+1] - peaks->idx_final[k]);
        
        peaks->slope.left[k]  = std::max(slopeL, slopeR);
        peaks->slope.right[k] = meanValue(slopeL, slopeR);
        
        peaks->spread.left[k]  = peaks->idx_final[k] - valleys->idx_final[k];
        peaks->spread.right[k] = valleys->idx_final[k+1] - peaks->idx_final[k];
    }
}
