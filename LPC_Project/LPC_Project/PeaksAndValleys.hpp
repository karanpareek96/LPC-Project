//
//  PeaksValleys.hpp
//  PracticeCode
//
//  Created by Karan Pareek on 11/07/19.
//  Copyright © 2019 Karan Pareek. All rights reserved.
//

#ifndef PeaksValleys_hpp
#define PeaksValleys_hpp

#define FREQ_RESP_LEN 256

#include <iostream>
#include <vector>


/* ------------------------------------------------------- */
/* -----------    Peak and Valley structs  --------------- */
/* ------------------------------------------------------- */

/*
 (sub)structs
 */

typedef struct {
    float left[FREQ_RESP_LEN];
    float right[FREQ_RESP_LEN];
} HEIGHT;

typedef struct {
    float left[FREQ_RESP_LEN];
    float right[FREQ_RESP_LEN];
} SPREAD;

typedef struct {
    float left[FREQ_RESP_LEN];
    float right[FREQ_RESP_LEN];
} SLOPE;

// Allocate a number

/*
 Peaks struct
 */

typedef struct {
//    float *idx;
    int *idx;
    int *idx_final;
    float *mag;
    float *mag_final;
    
    HEIGHT height;
    SPREAD spread;
    SLOPE slope;
} PEAKS;

/*
 Valleys struct
 */

typedef struct {
    int *idx;
    int *idx_final;
    float *mag;
    float *mag_final;
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
    void slopeNeg();
    
    /*
     Positive slope function
     */
    void slopePos();
    
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
    void analysis();
    
};

#endif /* PeaksValleys_hpp */
