//
//  RotarySequencer.hpp
//  DraaiLed2019
//
//  Created by Enrico Becker on 28/08/2019.
//
//

#ifndef RotarySequencer_hpp
#define RotarySequencer_hpp

#include <stdio.h>
#include "ofMain.h"

class RotarySequencer
{
public:
    struct Knop
    {
        ofVec2f position;
        float   radius;
        bool isStep;
        ofColor col;
        bool    pressed;
        int drawColorID;
    };
    
    RotarySequencer(ofRectangle area,float radius,int count,int id);
    ~RotarySequencer();
    
    void nextStep();
    float &updateDelta();
    void resetToBegin();
    void updateFbo();
    void update();
    void draw();
    
    
    void mousePressed(ofMouseEventArgs & args);
    ofEvent<int> trigger; //think about adding a listener
    //add, remove listener
    float &getDeltaTIme(){return delta;};
    vector<bool> &getSteps();
    void addListener(){ofAddListener(ofEvents().mousePressed, this, &RotarySequencer::mousePressed);};
    void removeListener(){ofRemoveListener(ofEvents().mousePressed, this, &RotarySequencer::mousePressed);};
    
    void setActive(bool value){isActive = value;};
    bool & getActive(){return isActive;};
    
private:
    ofRectangle     drawarea;
    ofFbo           drawFbo;
    int             myID;
    int             initSteps;
    vector<Knop>    steps;//later anothert struct with more options
    int             stepID;
    
    //new
    float lastStepTime;
    float stepTime;//holds the time per step
    float thisTriggerTime;
    float nextTriggerTime;
    float deltaTime;//holds the time when next trigger appears
    float delta;// the delta
    bool hasTrigger;//do we have a trigger?
    bool isActive;// is thi one visible enables mouse

};

#endif /* RotarySequencer_hpp */
