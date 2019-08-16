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
    void setColorA(int id);
    void setColorB(int id);
    void swapColor();
    void updateFBO();
    void draw();
    ofColor &getColorA(){return colors[colorIDA];};
    ofColor &getColorB(){return colors[colorIDB];};
    
    ofEvent<int> colorPressed;
    
private:
    ofRectangle     drawarea;
    ofFbo           fbo;
    int             colorIDA;
    int             colorIDB;
    int             xdim,ydim;
    ofRectangle     areaA[16];
    ofRectangle     areaB[16];
    
    ofRectangle     preview[2];
    ofRectangle     swap;
    

    ofColor colors[16] = {ofColor(0),ofColor(128),ofColor(255)
        ,ofColor(255,0,0),ofColor(255,128,0),ofColor(255,255,0)
        ,ofColor(128,255,0),ofColor(0,255,0),ofColor(0,255,128)
        ,ofColor(0,255,255),ofColor(0,128,255),ofColor(0,0,255)
        ,ofColor(128,0,255),ofColor(255,0,255),ofColor(255,0,128),ofColor(255,127,128)};

};

#endif /* ColorSwatch_hpp */
