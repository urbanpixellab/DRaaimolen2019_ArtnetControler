//
//  Zadar.hpp
//  Zadar
//
//  Created by Enrico Becker on 30/07/2019.
//
//

#ifndef Zadar_hpp
#define Zadar_hpp

#include <stdio.h>
#include "DataControler.hpp"

#define RES 100

class Zadar
{
public:
    enum CURVES
    {
        OFF = 0,
        ON,
        LINEAR,
        PWM,
        SAW,
        TRI,
        SINE,
        HALFSINE,
        ADSR,
        EASE,
        RANDOM1,
        RANDOM2,
        RANDOM3,
        ZIGZAG,
        ZIGZAG1,
        STAIRS,
        END
    };
    
    struct BUTTON
    {
        bool        pressed;
        string      name;
        ofRectangle drawarea;
        ofFbo       fbo;
        ofColor     color;
        int         id;
    };
    
    Zadar(ofRectangle drawarea,DataControler *data, ofTrueTypeFont *mFont);
    ~Zadar();
    
    void createGUI();
    float update();
    float update(float &dt);
    void draw();
    void drawGUI();
    
    void setCurve(int id);
    void setInverse(bool i){inverse = i;};
    void setReverse(bool r){reverse = r;};
    
    void addListener();
    void removeListener();
    void setModeButton(int id,bool value);
    void setCurveButton(int id, bool value);
    void mousePressed(ofMouseEventArgs & args);

    float &getValue(){return out;};
    
    bool &getInverse(){return inverse;};
    bool &getReverse(){return reverse;};
    int &getCurveID(){return curveID;};
    
    ofPixels &getPixels(){return curvePixels;};
    ofTexture &getCurveTex(){return curveImage.getTexture();};
    int getRightBorder(){return curvebuttons[curvebuttons.size()-1].drawarea.getRight();};

private:
    void createCurves();
    int myID;
    DataControler       *data;
    ofRectangle         drawarea;
    ofFbo               preview;//holds the curve preview
    int                 curveID;
    float               dT;
    float               out;
    
    vector<vector<float>>curveLUT;
    bool                inverse;// by -1
    bool                reverse;//end is begin!!!
    
    float               curveValues[100];//holds the
    ofPixels            curvePixels;//the gray texture of the image;
    ofImage             curveImage;
    
    //gui
    vector<BUTTON>      modebuttons;//invert reverse
    vector<BUTTON>      curvebuttons;// the curves
    ofColor             c[2] = {ofColor(0,0,255),ofColor(255,0,0)};

    ofTrueTypeFont      *mFont;
    string              curvenames[17] = {"OFF","ON","LINEAR","PWM","SAW","TRI","SINE","HALFSINE",
                                        "ADSR","EASE","RANDOM1","RANDOM2",
                                        "RANDOM3","ZIGZAG","ZIGZAG1","STAIRS"};
    
        //add ratio button for selct and two toggle sfor reverse and invert
};

#endif /* Zadar_hpp */