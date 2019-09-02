//
//  PatternEditor.cpp
//  Sequencer
//
//  Created by Enrico Becker on 05/08/2019.
//

#include "PatternEditor.hpp"

PatternEditor::PatternEditor(){}

PatternEditor::PatternEditor(ofRectangle area, ofTrueTypeFont *mFont)
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
    //pattern  segment generator
    for (int i = 0; i < 20; i++)
    {
        mPatSegGen[i] = new PatternGenerator(ofRectangle(x,y+h *1.1,w,h),4,mFont,"MIROR SUBSEGMENT");
        mPatSegGen[i]->addListener();

    }
    
    for (int i = 0; i < 16; i++)
    {
        mPatTexGen[i] = new PatternGenerator(ofRectangle(x,y+h *1.1,w,h),16,mFont,"MIROR Texture");
    }
    
    
    x = mPatGen->getRightBorder() + drawarea.getWidth() * 0.01;
    w = drawarea.getWidth() * 0.125;
    mCurve = new Zadar(ofRectangle(x,y,w,h),mFont,"MIRROR SEQUENZER BRIGHTNESS");

    /// now the segments elements
    y += h;
    x = drawarea.getLeft();
    w = drawarea.getWidth() * 0.25;

//    sSequenzer = new StepSequencer(ofRectangle(x,y,w,h),16,1);
//    ofAddListener(sSequenzer->trigger, this, &PatternEditor::sequenzerHit);
    
    x = drawarea.getLeft() + drawarea.getWidth() * 0.5;
    w = drawarea.getWidth() * 125;
//    sPatGen = new PatternGenerator(ofRectangle(x,y,w,h),SEGMENTS,mFont);
    
//    x = sPatGen->getRightBorder() + drawarea.getWidth() * 0.01;
//    w = drawarea.getWidth() * 0.24;
//    sCurve = new Zadar(ofRectangle(x,y,w,h),mFont);

    ////////// now the coloring stuff
    x = drawarea.getLeft();
    y = ofGetHeight() - 2*h;
    w = drawarea.getWidth() * 0.125;
    cPatGen = new PatternGenerator(ofRectangle(x,y,w,h),2,mFont,"COLOR SWAP");
    cPatGen->addListener();

    x = cPatGen->getRightBorder() + drawarea.getWidth() * 0.01;
    w = drawarea.getWidth() * 0.125;
    cCurve = new Zadar(ofRectangle(x,y,w,h),mFont,"COLOR CURVE");

    // color swatches
    x = drawarea.getLeft();
    y += 60;
    colors = new ColorSwatch(ofRectangle(x+w*1.3,y,120,60));
    ofAddListener(colors->colorPressed,this,&PatternEditor::colorPressed);

    colorsB = new ColorSwatch(ofRectangle(x+w*1.3,y+60,120,60));
    ofAddListener(colorsB->colorPressed,this,&PatternEditor::colorPressed);
    
    x = (ofGetWidth()-ofGetHeight())/2.0;
    y = 0;
    w = drawarea.getHeight();
    h = drawarea.getHeight();
    float s = 100;
    float rad = (w/2)*0.65;
    rotSequencer[0] = new RotarySequencer(ofRectangle(x,y,w,h),rad,16,1);
    rad *=0.65;
    s = 80;
    rotSequencer[1] = new RotarySequencer(ofRectangle(x+s,y+s,w-s*2,h-s*2),rad,16,2);
    ofAddListener(rotSequencer[0]->trigger, this, &PatternEditor::sequenzerHit);
    ofAddListener(rotSequencer[1]->trigger, this, &PatternEditor::sequenzerHit);


    setActive(false);
    //        rotSequencer[0]->addListener();
    //        rotSequencer[1]->addListener();


}

PatternEditor::~PatternEditor()
{
    //write data for storage
    //    these have to been called when a mouse input appears to keep the data updated
    int id = 0;
    //pData->writeSequencerSetting(id, mSequenzer->getSteps());
    //pData->writeAdsr(id, mCurve->getCurveID(), mCurve->getInverse(), mCurve->getReverse());
    //pData->writeMainPattern(id, mPatGen->getPatternID(), mPatGen->getSequenceDirection());
    delete mPatGen;
    delete mCurve;
//    delete sSequenzer;
//    delete sPatGen;
//    delete sCurve;
    delete cCurve;
    delete colors;
    for (int i = 0; i < 20; i++)
    {
        delete mPatSegGen[i];
    }
    delete cPatGen;
    delete rotSequencer[0];
    delete rotSequencer[1];



}

void PatternEditor::update()
{
    rotSequencer[0]->update();
    rotSequencer[1]->update();
    seqDelta[0] = mCurve->update(rotSequencer[0]->getDeltaTIme());
    seqDelta[1] = cCurve->update(rotSequencer[1]->getDeltaTIme());
}
                           
void PatternEditor::nextStep()
{
    //add update delta to rotary sequenzer
    rotSequencer[0]->nextStep();
    rotSequencer[1]->nextStep();

//    seqDelta[0] = mCurve->update(rotSequencer[0]->updateDelta());
    seqDelta[0] = mCurve->update(rotSequencer[0]->getDeltaTIme());
    
//    seqDelta[1] = sCurve->update(sSequenzer->updateDelta());
    seqDelta[1] = cCurve->update(rotSequencer[1]->getDeltaTIme());
}


void PatternEditor::drawGUI()
{
    if(!isActive) return;

    mCurve->draw();
    mPatGen->drawGUI();
    mPatSegGen[0]->drawGUI();//zeichne nur einnen sesub selector
    cPatGen->drawGUI();

//    sSequenzer->drawSequencer();
//    sCurve->draw();
//    sPatGen->drawGUI();
    cCurve->draw();
    colors->draw();
    colorsB->draw();
    rotSequencer[0]->draw();
    rotSequencer[1]->draw();

}

void PatternEditor::colorPressed(int &id)
{
    
}

void PatternEditor::sequenzerHit(int & index)
{
    
    cout << "sequenzer hit " << index << endl;
    if(index < 0) return;
    else if(index == 0)
    {
        //we have a mirror sequenzer  hit
        //update the mirrors
        
        mPatGen->updatePattern();
        for (int i = 0; i < 20; i++)
        {
            mPatSegGen[i]->updatePattern();
        }
        
    }
    else if(index == 1)
    {
        /*
        sPatGen->updatePattern();
        // get the enables of the segments based on the mirrors
        for (int m =0; m < mPatGen->getPattern().size(); m++) // tot 20
        {
            if(mPatGen->getPattern()[m] == true)
            {
                for (int s = 0; s < sPatGen->getPattern().size(); s++)
                {
                    segments[m*4 + s] = sPatGen->getPattern()[s];
                }
            }
            else
            {
                segments[m*4 + 0] = false;
                segments[m*4 + 1] = false;
                segments[m*4 + 2] = false;
                segments[m*4 + 3] = false;
            }
        }*/
    }
    else if(index == 2)
    {
        /// is comming
        //swap colors on demand
        
        // should be on a external button to ink to the sequencer
        //colors->swapColor();
    }
    ofNotifyEvent(isTrigger, index);

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
    for(int i = 0;i < 16;i++)
    {
        mPatTexGen[i]->setActive(isActive);
    }
    cCurve->setActive(isActive);
    cPatGen->setActive(isActive);
    colors->setActive(isActive);
    colorsB->setActive(isActive);
}
