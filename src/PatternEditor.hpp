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
#include "RotarySequencer.hpp"

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
    
    ofEvent<int> SequencerIDHit;
    
    ofColor getColorA1(){return colors->getColorA();};
    ofColor getColorA2(){return colors->getColorB();};
    
    float &getValueA(){return mCurve->getValue();};
    float &getValueC(){return cCurve->getValue();};
    
    vector<bool> &getMirrorPattern(){return mPatGen->getPattern();};
    vector<bool> &getMirrorSubPattern(int &spiegelID){return mPatSegGen[spiegelID]->getPattern();};//this returns all 4 segments per mirror
//    vector<bool> &getMirrorTexturePattern(int &spiegelID){return cPatGen->getPattern();};//this returns all 4 segments per mirror

    float &getColorDelta(){return rotSequencer[1]->getDeltaTIme();};
    
    void setActive(bool value);
    bool & getActive(){return isActive;};
    
    float &getColorFreq(){return cFreq->getValue();};
    float &getColorShift(){return cShift->getValue();};

    ofEvent<int> isTrigger;
    

private:
    ofRectangle         drawarea;
    float               seqDelta[2];

    PatternGenerator    *mPatGen;
    PatternGenerator    *mPatSegGen[20];
//    PatternGenerator    *cPatGen;// the cilor pattern
    
    //for every mirror we have a 4 pattern generator
    Zadar               *mCurve;

    Zadar               *cCurve; // make a second one later
    RotaryEncoder       *cFreq;//the frequency for the color curve 1
    RotaryEncoder       *cShift;//the phaseshift for color c
    ColorSwatch         *colors;//primary
    
    RotarySequencer         *rotSequencer[2];//mirror and color

    
//    bool    visible;
    bool    isActive;
};

#endif /* PatternEditor_hpp */
