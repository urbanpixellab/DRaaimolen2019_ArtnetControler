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
    
    void isVisible(bool value);
    ofEvent<int> SequencerIDHit;
    
    ofColor getColorA(){return colors->getColorA();};
    ofColor getColorB(){return colorsB->getColorB();};
    
    float &getDeltaA(){return mSequenzer->updateDelta();};
//    float &getDeltaB(){return sSequenzer->updateDelta();};
    float &getDeltaC(){return cSequenzer->updateDelta();};

    float &getValueA(){return mCurve->getValue();};
//    float &getValueB(){return sCurve->getValue();};
    float &getValueC(){return cCurve->getValue();};
    void colorPressed(int &id);
    
    vector<bool> &getMirrorPattern(){return mPatGen->getPattern();};
    vector<bool> &getMirrorSubPattern(int &spiegelID){return mPatSegGen[spiegelID]->getPattern();};//this returns all 4 segments per mirror
    vector<bool> &getMirrorTexturePattern(int &spiegelID){return mPatSegGen[spiegelID]->getPattern();};//this returns all 4 segments per mirror

    
    ofEvent<int> isTrigger;
    

private:
    ofRectangle         drawarea;
    float               seqDelta[3];

    StepSequencer       *mSequenzer;//mirror sequenzer
    PatternGenerator    *mPatGen;
    PatternGenerator    *mPatSegGen[20];
    //for every mirror we have a 4 pattern generator
    PatternGenerator    *mPatTexGen[16];//for every mirror we have a 4
    Zadar               *mCurve;

//    StepSequencer       *sSequenzer;//segment sequenzer
//    PatternGenerator    *sPatGen;
//    Zadar               *sCurve;

    StepSequencer       *cSequenzer;//color pattern sequenzer
    PatternGenerator    *cPatGen;///who two colors applied
    Zadar               *cCurve;

    ColorSwatch         *colors;//primary
    ColorSwatch         *colorsB;//primary
    
    bool    visible;
};

#endif /* PatternEditor_hpp */
