//
//  StepSequencer.hpp
//  Sequencer
//
//  Created by Enrico Becker on 05/08/2019.
//

#ifndef StepSequencer_hpp
#define StepSequencer_hpp

#include <stdio.h>
#include "ofMain.h"

class StepSequencer
{
public:
    struct STEP
    {
        ofRectangle drawarea;
        bool pressed;
        bool isStep;
        int drawColorID;
    };
    
    StepSequencer(ofRectangle drawarea,int maxStep,int id);
    ~StepSequencer();
    
    void update();// do the
    float &updateDelta();
    void drawSequencer();
    void nextStep();
    void resetToBegin();
    void mousePressed(ofMouseEventArgs & args);
    ofEvent<int> trigger; //think about adding a listener
    //add, remove listener
    float &getDeltaTIme(){return delta;};
    vector<bool> &getSteps();
    void addListener(){ofAddListener(ofEvents().mousePressed, this, &StepSequencer::mousePressed);};
    void removeListener(){ofRemoveListener(ofEvents().mousePressed, this, &StepSequencer::mousePressed);};
    
    void setActive(bool value){isActive = value;};
    bool & getActive(){return isActive;};

    
private:
    void createSequencer(ofRectangle drawarea,int maxStep);
    
    ofRectangle area;
    vector<STEP> steps; // this
    vector<bool> triggerSets;
    ofFbo buttons[2];
    ofColor color[3] = {ofColor(55),ofColor(255,255,0),ofColor(128,128,255)};
    int myID;
    int stepID; // holds the actual step
    float lastStepTime;
    float stepTime;//holds the time per step
    float thisTriggerTime;
    float nextTriggerTime;
    float deltaTime;//holds the time when next trigger appears
    float delta;// the delta
    bool hasTrigger;//do we have a trigger?
    int         radius;
    bool isActive;
};

#endif /* StepSequencer_hpp */
