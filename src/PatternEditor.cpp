//
//  PatternEditor.cpp
//  Sequencer
//
//  Created by Enrico Becker on 05/08/2019.
//

#include "PatternEditor.hpp"

PatternEditor::PatternEditor(){}

PatternEditor::PatternEditor(ofRectangle area, int ID,ofTrueTypeFont *mFont):myID(ID)
{
    drawarea = area;
    int x = drawarea.getLeft();
    int y = drawarea.getTop();
    int w = drawarea.getWidth() * 0.25;
    int h = drawarea.getHeight()/8; //divide into 4 reagions
    x = drawarea.getLeft();
    y += h*2;
    w = drawarea.getWidth() * 0.125;
    mPatGen = new PatternGenerator(ofRectangle(x,y,w,h),MIRRORS,mFont,"MIROR SELECT");
    mPatGen->addListener();
//    cPatGen = new PatternGenerator(ofRectangle(x,drawarea.getHeight() - h,w,h),MIRRORS,mFont,"MIROR COLOR");
//    cPatGen->addListener();
    //pattern  segment generator
    for (int i = 0; i < 20; i++)
    {
        mPatSegGen[i] = new PatternGenerator(ofRectangle(x,y+h *1.1,w,h),4,mFont,"MIROR SUBSEGMENT");
        mPatSegGen[i]->addListener();
    }
    
    
    x = mPatGen->getRightBorder() + drawarea.getWidth() * 0.01;
    w = drawarea.getWidth() * 0.125;
    mCurve = new Zadar(ofRectangle(x,y,w,h),mFont,"MIRROR SEQUENZER BRIGHTNESS");

    /// now the segments elements
    y += h;
    x = drawarea.getLeft();
    w = drawarea.getWidth() * 0.25;

    
    x = drawarea.getLeft() + drawarea.getWidth() * 0.5;
    w = drawarea.getWidth() * 125;
    
    ////////// now the coloring stuff
    x = drawarea.getLeft();
    y = ofGetHeight() - 3*h;
    w = drawarea.getWidth() * 0.125;
//    cPatGen = new PatternGenerator(ofRectangle(x,y,w,h),2,mFont,"COLOR SWAP");
//    cPatGen->addListener();

//    x = cPatGen->getRightBorder() + drawarea.getWidth() * 0.01;
    x = x+w + drawarea.getWidth() * 0.01;
    w = drawarea.getWidth() * 0.125;
    cCurve = new Zadar(ofRectangle(x,y,w,h),mFont,"COLOR CURVE");
    y += h*1.3;
    phaseModCurve = new Zadar(ofRectangle(x,y,w,h),mFont,"PHASE SHIFT CURVE");
    //ofRectangle area,int id,ofTrueTypeFont *f, string name,float min, float max,int sDraw,bool stick
    cFreq = new RotaryEncoder(ofRectangle(x-w,y,w/2,h),0,mFont,"Freq_C1",1,50,10,false);
    cShift = new RotaryEncoder(ofRectangle(x-w/2,y,w/2,h),0,mFont,"Shift_C1",-1,1,20,true);
    //add listeners
    
    // color swatches
    x = drawarea.getLeft();
    //y += 60;
    y = ofGetHeight() - 3*h;
    colors = new ColorSwatch(ofRectangle(x,y,200,100));

    x = (ofGetWidth()-ofGetHeight())/2.0;
    y = 0;
    w = drawarea.getHeight();
    h = drawarea.getHeight();
    float s = 100;
    float rad = (w/2)*0.65;
    rotSequencer[0] = new RotarySequencer(ofRectangle(x,y,w,h),rad,16,0,ofColor::wheat);
    rad *=0.65;
    s = 80;
    rotSequencer[1] = new RotarySequencer(ofRectangle(x+s,y+s,w-s*2,h-s*2),rad,16,1,ofColor::paleGoldenRod);
    ofAddListener(rotSequencer[0]->trigger, this, &PatternEditor::sequenzerHit);
    ofAddListener(rotSequencer[1]->trigger, this, &PatternEditor::sequenzerHit);
    
    setActive(false);

}

PatternEditor::~PatternEditor()
{
    //write data for storage
    //    these have to been called when a mouse input appears to keep the data updated
    int id = 0;
    delete mPatGen;
    delete mCurve;
    delete cCurve;
    delete phaseModCurve;
    delete cFreq;
    delete cShift;
    delete colors;
    for (int i = 0; i < 20; i++)
    {
        delete mPatSegGen[i];
    }
//    delete cPatGen;
    delete rotSequencer[0];
    delete rotSequencer[1];



}

void PatternEditor::update()
{
    rotSequencer[0]->update();
    rotSequencer[1]->update();
    seqDelta[0] = mCurve->update(rotSequencer[0]->getDeltaTIme());
    seqDelta[1] = cCurve->update(rotSequencer[1]->getDeltaTIme());
    phaseModCurve->update(rotSequencer[1]->getDeltaTIme());
}
                           
void PatternEditor::nextStep(int masterStepID)
{
    //add update delta to rotary sequenzer
    rotSequencer[0]->nextStep(masterStepID);
    rotSequencer[1]->nextStep(masterStepID);
}


void PatternEditor::drawGUI()
{
    if(!isActive) return;

    mCurve->draw();
    mPatGen->drawGUI();
    mPatSegGen[0]->drawGUI();//zeichne nur einnen sesub selector
//    cPatGen->drawGUI();

    cCurve->draw();
    colors->draw();
    rotSequencer[0]->draw();
    rotSequencer[1]->draw();
    
    cFreq->draw();
    cShift->draw();
    phaseModCurve->draw();

}

void PatternEditor::sequenzerHit(int & index)
{
//    cout << "pat trigger " << index << endl;
//    if(index < 0 || isActive == false) return;
    if(index == 0)
    {
        //we have a mirror sequenzer  hit
        mPatGen->updatePattern();
        for (int i = 0; i < 20; i++)
        {
            mPatSegGen[i]->updatePattern();
        }
    }
    ofNotifyEvent(isTrigger, myID);

}

void PatternEditor::setActive(bool value)
{
    isActive = value;
    //set all sub patterns active
    rotSequencer[0]->setActive(isActive);
    rotSequencer[1]->setActive(isActive);
    mPatGen->setActive(isActive);
    mCurve->setActive(isActive);
    for(int i = 0;i < 20;i++)
    {
        mPatSegGen[i]->setActive(isActive);
    }
    cCurve->setActive(isActive);
    cFreq->setActive(isActive);
    cShift->setActive(isActive);
    phaseModCurve->setActive(isActive);
    //    cPatGen->setActive(isActive);
    colors->setActive(isActive);
}
