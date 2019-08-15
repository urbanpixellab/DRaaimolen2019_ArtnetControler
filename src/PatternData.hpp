//
//  PatternData.hpp
//  Sequencer
//
//  Created by Enrico Becker on 01/08/2019.
//  holds the pattern data

#ifndef PatternData_hpp
#define PatternData_hpp

#include <stdio.h>
#include "ofMain.h"

class PatternData
{
public:
    PatternData(){};
    ~PatternData(){};
    ///sequenzer
    struct STEP
    {
        ofRectangle drawarea;
        bool pressed;
        bool isStep;
        int drawColorID;
    };
    
    //sequenzer
    STEP Sequenzer[3][16];//all 3 sequenzer
    int stepID[3] = {0,0,0};
    float lastStepTime[3];
    float stepTime[3];//holds the time per step
    float thisTriggerTime[3];
    float nextTriggerTime[3];
    float deltaTime[3];//holds the time when next trigger appears
    float delta[3];// the delta

    
    int CurveSelect[3];
    bool mCurveInv;
    bool mCurveRev;
};

#endif /* PatternData_hpp */
