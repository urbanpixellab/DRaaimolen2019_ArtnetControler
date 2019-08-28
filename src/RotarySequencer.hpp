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
        ofColor col;
        bool    pressed;
    };
    
    RotarySequencer(ofRectangle area,int count);
    ~RotarySequencer();
    
    void nextStep();
    void update();
    void draw();
    
    
    void mousePressed(ofMouseEventArgs & args);
    ofEvent<int> trigger; //think about adding a listener
    //add, remove listener
    float &getDeltaTIme(){return delta;};
    vector<bool> &getSteps();
    void addListener(){ofAddListener(ofEvents().mousePressed, this, &RotarySequencer::mousePressed);};
    void removeListener(){ofRemoveListener(ofEvents().mousePressed, this, &RotarySequencer::mousePressed);};
    
private:
    ofRectangle     drawarea;
    ofFbo           drawFbo;
    int             initSteps;
    vector<Knop>    steps;//later anothert struct with more options
    int             stepID;
    float           delta;// the delta
    
    
};

#endif /* RotarySequencer_hpp */
