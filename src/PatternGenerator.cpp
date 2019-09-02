//
//  PatternGenerator.cpp
//  Sequencer
//
//  Created by Enrico Becker on 05/08/2019.
//

#include "PatternGenerator.hpp"
PatternGenerator::PatternGenerator(ofRectangle area,int maxSeg, ofTrueTypeFont *f,string name) : mFont(f),myName(name),drawarea(area)
{
    maxSegment = maxSeg; // for all mirrors
    createGUI();
    setDirButton(0, true);
    isActive = false;
    setPattern(0);
}

PatternGenerator::~PatternGenerator()
{
    delete patternSelect;
}

void PatternGenerator::addListener()
{
    ofAddListener(ofEvents().mousePressed, this, &PatternGenerator::mousePressed);
    ofAddListener(patternSelect->newValue, this, &PatternGenerator::newEncoderID);
}

void PatternGenerator::removeListener()
{
    ofRemoveListener(ofEvents().mousePressed, this, &PatternGenerator::mousePressed);
    ofRemoveListener(patternSelect->newValue, this, &PatternGenerator::newEncoderID);
}


void PatternGenerator::createGUI()
{
    //we make a grid left rot encoder right buttons
    int w = drawarea.getWidth()/2;
    int h = drawarea.getHeight();
    int x = drawarea.getLeft();
    int y = drawarea.getTop();

    
    
    patternSelect = new RotaryEncoder(ofRectangle(x,y,w,h), 0, mFont, "PatternSelect", 0, 9, 9, true,patternnames);

    h /= 3;
    
    //top are the direction
    dirbuttons.clear();
    BUTTON f;
    f.id = 0;
    f.pressed = false;
    f.color = c[0];
    f.name = "FORWARD";
    f.drawarea = ofRectangle(drawarea.getRight()-w,drawarea.getTop(),w,h-2);
    f.fbo.allocate(w, h,GL_RGBA);
    f.fbo.begin();
    ofClear(0,0,0);
    ofSetColor(f.color);
    ofDrawRectRounded(0,0,w,h, 5);
    ofSetColor(255);
    ofRectangle fBbox = mFont->getStringBoundingBox(f.name, 0, 0);
    mFont->drawString(f.name, f.fbo.getWidth()/2 - fBbox.getWidth()/2, f.fbo.getHeight()/2 - fBbox.getHeight()/2);
    f.fbo.end();
    dirbuttons.push_back(f);
    
    BUTTON r;
    r.id = 0;
    r.pressed = false;
    r.color = c[0];
    r.name = "REVERSE";
    r.drawarea = ofRectangle(drawarea.getRight()-w,drawarea.getTop()+h,w,h-2);
    r.fbo.allocate(w, h,GL_RGBA);
    r.fbo.begin();
    ofClear(0,0,0);
    ofSetColor(r.color);
    ofDrawRectRounded(0,0,w,h, 5);
    ofSetColor(255);
    fBbox = mFont->getStringBoundingBox(r.name, 0, 0);
    mFont->drawString(r.name, r.fbo.getWidth()/2 - fBbox.getWidth()/2, r.fbo.getHeight()/2 - fBbox.getHeight()/2);
    r.fbo.end();
    dirbuttons.push_back(r);
    
    invPattern.id = 20;
    invPattern.color = c[0];
    invPattern.pressed = false;
    invPattern.name = "INVERT PATTERN";
    
    invPattern.drawarea = ofRectangle(drawarea.getRight()-w,drawarea.getTop()+h*2,w,h-2);
    invPattern.fbo.allocate(w, h,GL_RGBA);
    invPattern.fbo.begin();
    ofClear(0,0,0);
    ofSetColor(invPattern.color);
    ofDrawRectRounded(0,0,w,h, 5);
    ofSetColor(255);
    fBbox = mFont->getStringBoundingBox(invPattern.name, 0, 0);
    mFont->drawString(invPattern.name, invPattern.fbo.getWidth()/2 - fBbox.getWidth()/2, invPattern.fbo.getHeight()/2 - fBbox.getHeight()/2);
    
    patternSelect->draw();
    
    invPattern.fbo.end();
}

void PatternGenerator::drawGUI()
{
    if(!isActive) return;
    ofSetColor(255);
    mFont->drawString(myName, drawarea.getLeft(), drawarea.getTop() - 10);
    patternSelect->draw();
    dirbuttons[SDIR::FORWARD].fbo.draw(dirbuttons[SDIR::FORWARD].drawarea);
    dirbuttons[SDIR::REVERSE].fbo.draw(dirbuttons[SDIR::REVERSE].drawarea);
    invPattern.fbo.draw(invPattern.drawarea);
}


void PatternGenerator::updatePattern()
{
    int len = patternOut.size();

    switch (seqDirection)
    {
        case 0:
            sequenzCounter++;
            if (sequenzCounter >= maxSeqCount) sequenzCounter = 0;
            break;
            
        case 1:
            sequenzCounter--;
            if (sequenzCounter < 0) sequenzCounter = maxSeqCount-1;
            break;
            
        case 2:
            // think about it
            break;
            
            
        default:
            break;
    }

    switch (patternID) {
        case PATTERNS::ALL_OFF:
        {
            for (int i = 0; i < len; i++)
            {
                patternOut[i] = 0;
            }
            break;
        }
        case PATTERNS::ALL_ON:
        {
            for (int i = 0; i < len; i++)
            {
                patternOut[i] = 1;
            }
            break;
        }
        case PATTERNS::ON_OFF:
        {
            bool onOrOff = patternOut[0];
            for (int i = 0; i < len; i++)
            {
                patternOut[i] = !onOrOff;
            }
            break;
        }
        case PATTERNS::LAUFLICHT:
        {
            for (int j = 0; j < len; j++)
            {
                if(j == sequenzCounter) patternOut[j] = 1;
                else patternOut[j] = 0;
            }
            break;
        }
        case PATTERNS::CROSS:
        {
            //check even or odd!!!
            for (int j = 0; j < len; j++)
            {
                if(j == sequenzCounter || j == len-1-sequenzCounter) patternOut[j] = 1;
                else patternOut[j] = 0;
            }
            break;
        }
        case PATTERNS::OUT_IN:
        {
            //check even or odd!!!
            for (int j = 0; j < len; j++)
            {
                if(j == sequenzCounter || j == len-1-sequenzCounter) patternOut[j] = 1;
                else patternOut[j] = 0;
            }
            break;
        }
        case PATTERNS::RANDOM:
        {
            ofSeedRandom();
            for (int j = 0; j < len; j++)
            {
                int val = ofRandom(255);
                if(val < 128) patternOut[j] = 0;
                else patternOut[j] = 1;
            }
            break;
        }
        case PATTERNS::TURING:
        {
            // check for direction
            int val = ofRandom(255);
            if(val < 128) val = 0;
            else val = 1;
            if(seqDirection == SDIR::FORWARD)
            {
                patternOut.insert(patternOut.begin(), val);
                patternOut.pop_back();
            }
            else if(seqDirection == SDIR::REVERSE)
            {
                patternOut.push_back(val);
                patternOut.erase(patternOut.begin());
            }
            break;
        }
        case PATTERNS::SECOND:
        {
            for (int j = 0; j < len; j++)
            {
                if((j+sequenzCounter)%2 == 0) patternOut[j] = 1;
                else patternOut[j] = 0;
            }
            break;
        }
        case PATTERNS::THIRD:
        {
            for (int j = 0; j < len; j++)
            {
                if((j+sequenzCounter)%3 == 0) patternOut[j] = 1;
                else patternOut[j] = 0;
            }
            break;
        }
        default: //inverse all
        {
            break;
        }
    }

    if(invPattern.pressed) //inverse the selection
    {
        maxSeqCount = maxSegment;
        for (int j = 0; j < maxSegment; j++)
        {
            patternOut[j] = !patternOut[j];
        }
    }
    //printSequence();
}

void PatternGenerator::setPattern(int patSelect)
{
    //fills the outpattern once and set the max step length
    // then updated in updatepattern
    patternID = patSelect;
    
    //check for even odd
    //maxSegment%2 == 0 is even
    switch (patternID) {
        case PATTERNS::ALL_OFF:
        {
            patternOut.clear();
            maxSeqCount = 1;
            for (int i = 0; i < maxSegment; i++)
            {
                patternOut.push_back(0);
            }
            break;
        }
        case PATTERNS::ALL_ON:
        {
            patternOut.clear();
            maxSeqCount = 1;
            for (int i = 0; i < maxSegment; i++)
            {
                patternOut.push_back(1);
            }
            break;
        }
        case PATTERNS::ON_OFF:
        {
            patternOut.clear();
            maxSeqCount = 2;
            for (int i = 0; i < maxSegment; i++)
            {
                patternOut.push_back(0);
            }
            break;
        }
        case PATTERNS::LAUFLICHT:
        {
            patternOut.clear();
            maxSeqCount = maxSegment;
            
            for (int j = 0; j < maxSegment; j++)
            {
                if(j == 0) patternOut.push_back(1);
                else patternOut.push_back(0);
            }
            break;
        }
        case PATTERNS::CROSS:
        {
            patternOut.clear();
            //check even or odd!!!
            maxSeqCount = maxSegment;
            
            for (int j = 0; j < maxSegment; j++)
            {
                if(j == 0 || j == maxSegment-1) patternOut.push_back(1);
                else patternOut.push_back(0);
            }
            break;
        }
        case PATTERNS::OUT_IN:
        {
            patternOut.clear();
            //check even or odd!!!
            maxSeqCount = maxSegment/2;
            
            for (int j = 0; j < maxSegment; j++)
            {
                if(j == 0 || j == maxSegment-1) patternOut.push_back(1);
                else patternOut.push_back(0);
            }
            break;
        }
        case PATTERNS::RANDOM:
        {
            patternOut.clear();
            maxSeqCount = -1;
            for (int j = 0; j < maxSegment; j++)
            {
                int val = ofRandom(255);
                if(val < 128) patternOut.push_back(0);
                else patternOut.push_back(1);
            }
            break;
        }
        case PATTERNS::TURING:
        {
            patternOut.clear();
            maxSeqCount = -1;
            for (int j = 0; j < maxSegment; j++)
            {
                int val = ofRandom(255);
                if(val < 128) patternOut.push_back(0);
                else patternOut.push_back(1);
            }
            break;
        }
        case PATTERNS::SECOND:
        {
            maxSeqCount = 2;
            patternOut.clear();
            for (int j = 0; j < maxSegment; j++)
            {
                if(j%2 == 0) patternOut.push_back(1);
                else patternOut.push_back(0);
            }
            break;
        }
        case PATTERNS::THIRD:
        {
            maxSeqCount = 4;
            patternOut.clear();
            for (int j = 0; j < maxSegment; j++)
            {
                if(j%5 == 0) patternOut.push_back(1);
                else patternOut.push_back(0);
            }
            break;
        }
        default:
            break;
    }
    sequenzCounter = 0;
    //printSequence();
}

void PatternGenerator::printSequence()
{
    string s = "sequence " + ofToString(sequenzCounter) + " ";
    for (int i = 0; i < patternOut.size(); i++)
    {
        s += ":" + ofToString(patternOut[i]);
    }
    cout << s << endl;
}

void PatternGenerator::newEncoderID(int & id)
{
    setPattern(patternSelect->getValue());
}

void PatternGenerator::mousePressed(ofMouseEventArgs & args)
{
    if(!isActive) return;

    for (int i = 0; i < dirbuttons.size();i++)
    {
        if(dirbuttons[i].drawarea.inside(args.x, args.y))
        {
            setDirButton(i, !dirbuttons[i].pressed);
            return;
        }
    }
    if(invPattern.drawarea.inside(args.x,args.y))
    {
        setInverseButton(!invPattern.pressed);
    }
}

void PatternGenerator::setDirButton(int id,bool value)
{
    dirbuttons[id].pressed = value;
    // now set the state
    dirbuttons[id].color = c[dirbuttons[id].pressed];
    dirbuttons[id].fbo.begin();
    ofClear(0,0,0);
    ofSetColor(dirbuttons[id].color);
    ofDrawRectRounded(0,0,dirbuttons[id].drawarea.getWidth(),dirbuttons[id].drawarea.getHeight(), 5);
    ofSetColor(255);

    ofRectangle fBbox = mFont->getStringBoundingBox(dirbuttons[id].name, 0, 0);
    mFont->drawString(dirbuttons[id].name, dirbuttons[id].fbo.getWidth()/2 - fBbox.getWidth()/2, dirbuttons[id].fbo.getHeight()/2 + fBbox.getHeight()/2);
    dirbuttons[id].fbo.end();
    setPatternDirection(id);
    id = !id;
    
    dirbuttons[id].pressed = !value;
    // now set the state
    dirbuttons[id].color = c[dirbuttons[id].pressed];
    dirbuttons[id].fbo.begin();
    ofClear(0,0,0);
    ofSetColor(dirbuttons[id].color);
    ofDrawRectRounded(0,0,dirbuttons[id].drawarea.getWidth(),dirbuttons[id].drawarea.getHeight(), 5);
    ofSetColor(255);
    fBbox = mFont->getStringBoundingBox(dirbuttons[id].name, 0, 0);
    mFont->drawString(dirbuttons[id].name, dirbuttons[id].fbo.getWidth()/2 - fBbox.getWidth()/2, dirbuttons[id].fbo.getHeight()/2 + fBbox.getHeight()/2);
//    mFont->drawString(dirbuttons[id].name, 5, dirbuttons[id].drawarea.getHeight() - 2);
    dirbuttons[id].fbo.end();
}

void PatternGenerator::setInverseButton(bool pressed)
{
    invPattern.pressed = pressed;
    invPattern.color = c[pressed];
    invPattern.fbo.begin();
    ofClear(0,0,0);
    ofSetColor(invPattern.color);
    ofDrawRectRounded(0,0,invPattern.drawarea.getWidth(),invPattern.drawarea.getHeight(), 5);
    ofSetColor(255);
    ofRectangle fBbox = mFont->getStringBoundingBox(invPattern.name, 0, 0);
    mFont->drawString(invPattern.name, invPattern.fbo.getWidth()/2 - fBbox.getWidth()/2, invPattern.fbo.getHeight()/2 + fBbox.getHeight()/2);
    
//    mFont->drawString(invPattern.name, 5, invPattern.drawarea.getHeight() - 2);
    invPattern.fbo.end();
}

void PatternGenerator::setActive(bool value)
{
    isActive = value;
    patternSelect->setActive(isActive);
}
