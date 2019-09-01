//
//  RotaryEncoder.hpp
//
//
//  Created by Enrico Becker on 27/08/2019.
//

#ifndef RotaryEncoder_hpp
#define RotaryEncoder_hpp

#include <stdio.h>
#include "ofMain.h"

class RotaryEncoder
{
public:
    RotaryEncoder(ofRectangle area,int id,ofTrueTypeFont *f, string name,float min, float max,int sDraw,bool stick,string names[12]);
    RotaryEncoder(ofRectangle area,int id,ofTrueTypeFont *f, string name,float min, float max,int sDraw,bool stick);
    ~RotaryEncoder();
    
    void updateFbo();
    void draw();
    
    void setRange(ofVec2f r);
    void setValue(float value);
    
    void mousePressed(ofMouseEventArgs &args);
    void mouseDragged(ofMouseEventArgs &args);
    void mouseReleased(ofMouseEventArgs &args);
    
    void addListener();
    void removeListener();
    
    float &getValueNormalized(){return value;};
    float &getValue(){return mapValue;};
    
    void setActive(bool value){isActive = value;};
    bool & getActive(){return isActive;};

    
    ofEvent<int> newValue;

private:
    ofRectangle 	    drawarea;
    ofTrueTypeFont      *mFont;
    int                 myID;
    string              myName;
    int                 stepsDrawn;// how many lines are drawn
    ofFbo               drawFbo;
    bool                isSticking;//sticks to the lines
    
    float               value;
    float               mapValue;
    ofVec2f             range;
    
    ofVec2f             cCenter; //circle center
    float               cRadius; //circle radius
    ofVec2f             fBegin;//the begin of the text
    ofVec2f             mouseStart;
    bool                isSliding = false;
    float               clickValue;// holds the pressed value
    vector<ofVec4f>     lines;
    bool                isActive;
    string              names[10]={""};
};


#endif /* RotaryEncoder_hpp */
