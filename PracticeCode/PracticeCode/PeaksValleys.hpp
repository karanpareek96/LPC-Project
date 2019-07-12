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
    std::vector<float> left;
    std::vector<float> right;
} HEIGHT;

typedef struct {
    std::vector<float> left;
    std::vector<float> right;
} SPREAD;

typedef struct {
    std::vector<float> left;
    std::vector<float> right;
} SLOPE;

/*
 Peaks struct
 */

typedef struct {
    std::vector<float> idx;
    std::vector<float> mag;
    
    HEIGHT height;
    SPREAD spread;
    SLOPE slope;
} PEAKS;

/*
 Valleys struct
 */

typedef struct {
    std::vector<float> idx;
    std::vector<float> mag;
} VALLEYS;


/* ------------------------------------------------------- */
/* -----------    Peak and Valley class  ----------------- */
/* ------------------------------------------------------- */

class PeaksAndValleys{
    
public:
    
    unsigned int valleyPair;
    unsigned int cntValley;
    unsigned int cntPeak;
    unsigned int prevSlopePos;
    bool init;
    int len;
    float **sigPointer;
    
    HEIGHT *height;
    SPREAD *spread;
    SLOPE *slope;
    PEAKS *peaks;
    VALLEYS *valleys;
    
    
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
    void analysis();

};

#endif /* PeaksValleys_hpp */
