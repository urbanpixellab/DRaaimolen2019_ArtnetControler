//
//  ColorSwatch.hpp
//  Sequencer
//
//  Created by Enrico Becker on 06/08/2019.
//

#ifndef ColorSwatch_hpp
#define ColorSwatch_hpp

#include <stdio.h>
#include "ofMain.h"

class ColorSwatch
{
public:
    ColorSwatch(ofRectangle draw);
    ~ColorSwatch();

    void mousePressed(ofMouseEventArgs & args);
    void addListener(){ofAddListener(ofEvents().mousePressed, this, &ColorSwatch::mousePressed);};
    void removeListener(){ofRemoveListener(ofEvents().mousePressed, this, &ColorSwatch::mousePressed);};
    void setColor(int id);
    void updateFBO();
    void draw();
    ofColor &getColor(){return colors[colorID];};
    
    ofEvent<int> colorPressed;
    
private:
    ofRectangle     drawarea;
    ofFbo           fbo;
    int             colorID;
    int             xdim,ydim;
    ofRectangle     area[16];
    
    ofColor colors[16] = {ofColor(0),ofColor(128),ofColor(255)
        ,ofColor(255,0,0),ofColor(255,128,0),ofColor(255,255,0)
        ,ofColor(128,255,0),ofColor(0,255,0),ofColor(0,255,128)
        ,ofColor(0,255,255),ofColor(0,128,255),ofColor(0,0,255)
        ,ofColor(128,0,255),ofColor(255,0,255),ofColor(255,0,128),ofColor(255,127,128)};

};

#endif /* ColorSwatch_hpp */
