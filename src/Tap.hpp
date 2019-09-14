//
//  Tap.hpp
//
//
//  Created by Enrico Becker on 10/09/2019.
//

#ifndef Tap_hpp
#define Tap_hpp

#include <stdio.h>
#include "ofMain.h"

class Tap
{
public:
    Tap(){tapCount = 0;};
    Tap(ofRectangle area,ofTrueTypeFont *font);
    ~Tap();
    void draw();
    void mousePressed(ofMouseEventArgs & args);
    
    ofEvent<float>   newTimer;
    
private:
    string calculateBPM(float &d);
    
    ofRectangle     drawarea;
    ofTrueTypeFont *f;
    int             tapCount = 0;
    float           lastPressed;
    float           delta;
    bool            pressed;
    float           bpm;
    string          bpmString;
};


#endif /* Tap_hpp */
