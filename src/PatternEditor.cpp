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
    isVisible(false);
    drawarea = area;
    int x = drawarea.getLeft();
    int y = drawarea.getTop();
    int w = drawarea.getWidth() * 0.49;
    int h = drawarea.getHeight()/4; //divide into 4 reagions
    mSequenzer = new StepSequencer(ofRectangle(x,y,w,h),16,0);
    ofAddListener(mSequenzer->trigger, this, &PatternEditor::sequenzerHit);
    
    x = drawarea.getLeft() + drawarea.getWidth() * 0.5;
    w = drawarea.getWidth() * 0.24;
    mPatGen = new PatternGenerator(ofRectangle(x,y,w,h),MIRRORS,mFont);
    
    x = mPatGen->getRightBorder() + drawarea.getWidth() * 0.01;
    w = drawarea.getWidth() * 0.24;
    mCurve = new Zadar(ofRectangle(x,y,w,h),mFont);

    /// now the segments elements
    y += h;
    x = drawarea.getLeft();
    w = drawarea.getWidth() * 0.49;

    sSequenzer = new StepSequencer(ofRectangle(x,y,w,h),16,1);
    ofAddListener(sSequenzer->trigger, this, &PatternEditor::sequenzerHit);
    
    x = drawarea.getLeft() + drawarea.getWidth() * 0.5;
    w = drawarea.getWidth() * 0.24;
    sPatGen = new PatternGenerator(ofRectangle(x,y,w,h),SEGMENTS,mFont);
    
    x = sPatGen->getRightBorder() + drawarea.getWidth() * 0.01;
    w = drawarea.getWidth() * 0.24;
    sCurve = new Zadar(ofRectangle(x,y,w,h),mFont);

    ////////// now the coloring stuff
    y += h;
    x = drawarea.getLeft();
    w = drawarea.getWidth() * 0.49;
    
    cSequenzer = new StepSequencer(ofRectangle(x,y,w,h),16,2);
    ofAddListener(cSequenzer->trigger, this, &PatternEditor::sequenzerHit);
    
    x = sPatGen->getRightBorder() + drawarea.getWidth() * 0.01;
    w = drawarea.getWidth() * 0.24;
    cCurve = new Zadar(ofRectangle(x,y,w,h),mFont);
    // color swatches
    
    colorA = new ColorSwatch(ofRectangle(x+w*1.3,y,60,60));
    ofAddListener(colorA->colorPressed,this,&PatternEditor::colorPressed);
    colorB = new ColorSwatch(ofRectangle(x+w*1.7,y,60,60));
    ofAddListener(colorB->colorPressed,this,&PatternEditor::colorPressed);
}

PatternEditor::~PatternEditor()
{
    //write data for storage
    //    these have to been called when a mouse input appears to keep the data updated
    int id = 0;
    //pData->writeSequencerSetting(id, mSequenzer->getSteps());
    //pData->writeAdsr(id, mCurve->getCurveID(), mCurve->getInverse(), mCurve->getReverse());
    //pData->writeMainPattern(id, mPatGen->getPatternID(), mPatGen->getSequenceDirection());
    delete mSequenzer;
    delete mPatGen;
    delete mCurve;
    delete sSequenzer;
    delete sPatGen;
    delete sCurve;
    delete cSequenzer;
    delete cCurve;
    delete colorA;
    delete colorB;

}

void PatternEditor::update()
{
    seqDelta[0] = mCurve->update(mSequenzer->updateDelta());
    seqDelta[1] = sCurve->update(sSequenzer->updateDelta());
    seqDelta[2] = cCurve->update(cSequenzer->updateDelta());
}
                           
void PatternEditor::nextStep()
{
    mSequenzer->nextStep();
    sSequenzer->nextStep();
    cSequenzer->nextStep();
    seqDelta[0] = mCurve->update(mSequenzer->updateDelta());
    seqDelta[1] = sCurve->update(sSequenzer->updateDelta());
    seqDelta[2] = cCurve->update(cSequenzer->updateDelta());
}


void PatternEditor::drawGUI()
{
    mSequenzer->drawSequencer();
    mCurve->draw();
    mPatGen->drawGUI();
    sSequenzer->drawSequencer();
    sCurve->draw();
    sPatGen->drawGUI();
    cSequenzer->drawSequencer();
    cCurve->draw();
    colorA->draw();
    colorB->draw();
}

void PatternEditor::colorPressed(int &id)
{
    
}

void PatternEditor::sequenzerHit(int & index)
{
    
    if(index < 0) return;
    else if(index == 0)
    {
        //we have a mirror sequenzer  hit
        //update the mirrors
        
        mPatGen->updatePattern();
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
        }
    }
    else if(index == 1)
    {
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
        }
    }
    else if(index == 2)
    {
        /// is comming
    }
    ofNotifyEvent(isTrigger, index);

}

void PatternEditor::isVisible(bool value)
{
    visible = value;
    if(visible)
    {
        mSequenzer->addListener();//mirror sequenzer
        mPatGen->addListener();
        mCurve->addListener();
        sSequenzer->addListener();//mirror sequenzer
        sPatGen->addListener();
        sCurve->addListener();
        cSequenzer->addListener();//mirror sequenzer
        cCurve->addListener();
        colorA->addListener();
        colorB->addListener();
    }
    else
    {
        mSequenzer->removeListener();//mirror sequenzer
        mPatGen->removeListener();
        mCurve->removeListener();
        sSequenzer->removeListener();//segment sequenzer
        sPatGen->removeListener();
        sCurve->removeListener();
        cSequenzer->removeListener();//segment sequenzer
        cCurve->removeListener();
        colorA->removeListener();
        colorB->removeListener();
    }
}
