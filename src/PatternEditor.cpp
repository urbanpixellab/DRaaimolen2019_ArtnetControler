//
//  PatternEditor.cpp
//  Sequencer
//
//  Created by Enrico Becker on 05/08/2019.
//

#include "PatternEditor.hpp"

PatternEditor::PatternEditor(){}

PatternEditor::PatternEditor(ofRectangle area,DataControler *d, ofTrueTypeFont *mFont): pData(d)
{
    isVisible(false);
    drawarea = area;
    int x = drawarea.getLeft();
    int y = drawarea.getTop();
    int w = drawarea.getWidth() * 0.49;
    int h = drawarea.getHeight()/4; //divide into 4 reagions
    mSequenzer = new StepSequencer(ofRectangle(x,y,w,h),pData,16,0);
    ofAddListener(mSequenzer->trigger, this, &PatternEditor::sequenzerHit);
    
    x = drawarea.getLeft() + drawarea.getWidth() * 0.5;
    w = drawarea.getWidth() * 0.24;
    mPatGen = new PatternGenerator(ofRectangle(x,y,w,h),pData,MIRRORS,mFont);
    
    x = mPatGen->getRightBorder() + drawarea.getWidth() * 0.01;
    w = drawarea.getWidth() * 0.24;
    mCurve = new Zadar(ofRectangle(x,y,w,h),pData,mFont);

    /// now the segments elements
    y += h;
    x = drawarea.getLeft();
    w = drawarea.getWidth() * 0.49;

    sSequenzer = new StepSequencer(ofRectangle(x,y,w,h),pData,16,1);
    ofAddListener(sSequenzer->trigger, this, &PatternEditor::sequenzerHit);
    
    x = drawarea.getLeft() + drawarea.getWidth() * 0.5;
    w = drawarea.getWidth() * 0.24;
    sPatGen = new PatternGenerator(ofRectangle(x,y,w,h),pData,SEGMENTS,mFont);
    
    x = sPatGen->getRightBorder() + drawarea.getWidth() * 0.01;
    w = drawarea.getWidth() * 0.24;
    sCurve = new Zadar(ofRectangle(x,y,w,h),pData,mFont);

    ////////// now the coloring stuff
    y += h;
    x = drawarea.getLeft();
    w = drawarea.getWidth() * 0.49;
    
    cSequenzer = new StepSequencer(ofRectangle(x,y,w,h),pData,16,2);
    ofAddListener(cSequenzer->trigger, this, &PatternEditor::sequenzerHit);
    
    x = sPatGen->getRightBorder() + drawarea.getWidth() * 0.01;
    w = drawarea.getWidth() * 0.24;
    cCurve = new Zadar(ofRectangle(x,y,w,h),pData,mFont);
    // color swatches
    
    colorA = new ColorSwatch(ofRectangle(x+w*1.3,y,100,100));
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
    ofSetColor(testcolor*seqDelta[0]);
    ofDrawCircle(ofGetWidth()/2, ofGetHeight()/2, testradius*seqDelta[1]);
    mSequenzer->drawSequencer();
    mCurve->draw();
    mPatGen->drawGUI();
    sSequenzer->drawSequencer();
    sCurve->draw();
    sPatGen->drawGUI();
    cSequenzer->drawSequencer();
    cCurve->draw();
    colorA->draw();
}

void PatternEditor::sequenzerHit(int & index)
{
    if(index < 0) return;
    else if(index == 0)
    {
        testcolor = ofColor(ofRandom(255),ofRandom(255),ofRandom(255));
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
    }
}
