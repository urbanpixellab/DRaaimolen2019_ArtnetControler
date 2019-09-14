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
#include "ofxXmlSettings.h"

class ColorSwatch
{
public:
    ColorSwatch(ofRectangle draw);
    ~ColorSwatch();

    void mousePressed(ofMouseEventArgs & args);
    void setColorA(int id);
    void setColorB(int id);
    void swapColor();
    void updateFBO();
    void draw();
    ofColor &getColorA(){return colors[colorIDA];};
    ofColor &getColorB(){return colors[colorIDB];};
    void setActive(bool value){isActive = value;};
    bool & getActive(){return isActive;};
    void loadColors();

    int getColorAID(){return colorIDA;};
    int getColorBID(){return colorIDB;};
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
    ofRectangle     swapSeq;
    bool            isSeq = false;
    

    ofColor colors[16] = {ofColor(0),ofColor(128),ofColor(255)
        ,ofColor(255,0,0),ofColor(0,255,0),ofColor(0,0,255)
        ,ofColor(255,0,255),ofColor(0,252,250),ofColor(0,115,0)
        ,ofColor(57,2,113),ofColor(255,3,138),ofColor(0,255,200)
        ,ofColor(55,100,0),ofColor(245,100,160),ofColor(15,94,140),ofColor(232,44,12)};

    bool    isActive;
    
};

#endif /* ColorSwatch_hpp */
