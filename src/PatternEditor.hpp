//
//  PatternEditor.hpp
//  Sequencer
//
//  Created by Enrico Becker on 05/08/2019.
//

#ifndef PatternEditor_hpp
#define PatternEditor_hpp

#include <stdio.h>
#include "StepSequencer.hpp"
#include "PatternGenerator.hpp"
#include "Zadar.hpp"
#include "ColorSwatch.hpp"

#define MIRRORS 20
#define SEGMENTS 4

class PatternEditor
{
public:
    
    PatternEditor();
    PatternEditor(ofRectangle area, ofTrueTypeFont *mFont);
    ~PatternEditor();
    
    void nextStep();
    void update();
    void drawGUI();
    void sequenzerHit(int & index);
    
    ofTexture   &getCurve(){return cCurve->getCurveTex();};
    bool *getSegmentPattern(){return segments;};
    
    void isVisible(bool value);
    ofEvent<int> SequencerIDHit;
    
    ofColor getColorA(){return colorA->getColor();};
    ofColor &getColorB(){return colorB->getColor();};
    
    float &getDeltaA(){return mSequenzer->updateDelta();};
    float &getDeltaB(){return sSequenzer->updateDelta();};
    float &getDeltaC(){return cSequenzer->updateDelta();};

    float &getValueA(){return mCurve->getValue();};
    float &getValueB(){return sCurve->getValue();};
    float &getValueC(){return cCurve->getValue();};
    void colorPressed(int &id);
    
    vector<bool> &getMirrorPattern(){return mPatGen->getPattern();};
    
    ofEvent<int> isTrigger;
    

private:
    ofRectangle         drawarea;
    float               seqDelta[3];

    StepSequencer       *mSequenzer;//mirror sequenzer
    PatternGenerator    *mPatGen;
    Zadar               *mCurve;

    StepSequencer       *sSequenzer;//segment sequenzer
    PatternGenerator    *sPatGen;
    Zadar               *sCurve;

    StepSequencer       *cSequenzer;//color pattern sequenzer
    Zadar               *cCurve;

    ColorSwatch         *colorA;//primary
    ColorSwatch         *colorB;//primary
    
    bool    visible;
    bool    segments[80] = {false};// holds all segments
};

#endif /* PatternEditor_hpp */
