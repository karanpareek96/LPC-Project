//
//  PeaksValleys.hpp
//  PracticeCode
//
//  Created by Karan Pareek on 11/07/19.
//  Copyright © 2019 Karan Pareek. All rights reserved.
//

#ifndef PeaksValleys_hpp
#define PeaksValleys_hpp

#include <iostream>
#include <vector>


/* ------------------------------------------------------- */
/* -----------    Peak and Valley structs  --------------- */
/* ------------------------------------------------------- */

/*
 (sub)structs
 */

typedef struct {
    float *left;
    float *right;
} HEIGHT;

typedef struct {
    float *left;
    float *right;
} SPREAD;

typedef struct {
    float *left;
    float *right;
} SLOPE;

/*
 Peaks struct
 */

typedef struct {
    float *idx;
    float *mag;
    
    HEIGHT height;
    SPREAD spread;
    SLOPE slope;
} PEAKS;

/*
 Valleys struct
 */

typedef struct {
    float *idx;
    float *mag;
} VALLEYS;


/* ------------------------------------------------------- */
/* -----------    Peak and Valley class  ----------------- */
/* ------------------------------------------------------- */

class PeaksAndValleys{
    
public:
    
    // Main init variables
    unsigned int valleyPair;
    unsigned int cntValley;
    unsigned int cntPeak;
    unsigned int prevSlopePos;
    bool init;
    int len;
    float *sigPointer;
    
    // Pointer variables for script
    float *initValue; // Length: signalLength
    float *slopes;    // Length: signalLength-1
    
    PEAKS *peaks;
    VALLEYS *valleys;
//    HEIGHT *height;
//    SPREAD *spread;
//    SLOPE *slope;
    
    /*
     Constructor
     */
    PeaksAndValleys(float *signal, unsigned int signalLength);
    
    /*
     Destructor
     */
    ~PeaksAndValleys();
    
    /*
     Computation function
     */
    void computeParams(float *signal, unsigned int signalLength);
    
    /*
     Negative slope function
     */
    void slopeNeg(float *slopes);
    
    /*
     Positive slope function
     */
    void slopePos(float *slopes);
    
    /*
     Update Peaks
     */
    void updatePeaks(int k);
    
    /*
     Update Valleys
     */
    void updateValleys(int k);
    
    /*
     Peak and Valley analysis
     */
    void analysis(unsigned int cntPeak);
    
};

#endif /* PeaksValleys_hpp */
