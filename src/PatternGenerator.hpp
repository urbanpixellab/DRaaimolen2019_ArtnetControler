//
//  PatternGenerator.hpp
//  Sequencer
//
//  Created by Enrico Becker on 05/08/2019.
//

#ifndef PatternGenerator_hpp
#define PatternGenerator_hpp

#include <stdio.h>
#include "ofMain.h"
#include "RotaryEncoder.hpp"

class PatternGenerator
{
public:
    enum PATTERNS//holds the different sequences
    {
        ALL_OFF = 0,
        ALL_ON,
        ON_OFF,
        LAUFLICHT,
        CROSS,
        OUT_IN,
        RANDOM,// has to been generic duss calculation on each step
        TURING,// has to been generic duss calculation on each step
        SECOND,
        THIRD,
        END
        //OUT_IN_RND
        //////add some more patterns
    };
    
    enum SDIR // in which direction the sequence should go
    {
        FORWARD = 0,
        REVERSE
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
    
    
    
    PatternGenerator(ofRectangle area,int maxSeg, ofTrueTypeFont *f,string name);
    ~PatternGenerator();
    
    void updatePattern();// gets triggered by sequencer
    void setPattern(int id);
    void setPatternDirection(int d){seqDirection = d;};
    vector<bool> &getPattern(){return patternOut;};
    
    void printSequence();
    
    void createGUI();
    void drawGUI();
    void addListener();
    void removeListener();
    void mousePressed(ofMouseEventArgs & arg);
    void setDirButton(int i,bool pressed);
    void setInverseButton(bool pressed);
    
    int &getPatternID(){return patternID;};
    int &getSequenceDirection(){return seqDirection;};
    
    float getRightBorder(){return drawarea.getRight();};
    
    void newEncoderID(int & id);
    void setActive(bool value);
    bool & getActive(){return isActive;};
    RotaryEncoder getEncoder(){return *patternSelect;};
    
private:
    string myName;
    int maxSegment;//how many mirrors
    int patternID;//the selected pattern
    int sequenzerID; /// which sequenzer i belong to
    //sequencing stuff
    int sequenzCounter; //the counter for the sequenz which is used by the trigger
    int seqDirection; // holds the sequence direction
    int maxSeqCount;// holds the maximum lenth of the sequence
    
    vector<bool> patternOut;// thios holds the segments per step
    
    
    //gui
    ofTrueTypeFont      *mFont;
    ofRectangle         drawarea;
    vector<BUTTON>      dirbuttons;//the buttons for the direction;
    BUTTON              invPattern;

    ofColor             c[2] = {ofColor(0,0,255),ofColor(255,0,0)};
    int                 lastPressed;//save the last for invert
    string              patternnames[10] = {"OFF","ON","ON/OFF","RUN","CROSS","OUT_IN",
        "RANDOM","TURING","SECOND","THIRD"};
    RotaryEncoder       *patternSelect;
    
    bool    isActive;
};

#endif /* PatternGenerator_hpp */
