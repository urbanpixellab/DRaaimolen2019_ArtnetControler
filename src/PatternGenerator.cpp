//
//  PatternGenerator.cpp
//  Sequencer
//
//  Created by Enrico Becker on 05/08/2019.
//

#include "PatternGenerator.hpp"
PatternGenerator::PatternGenerator(ofRectangle area, DataControler *d,int maxSeg, ofTrueTypeFont *f) : mFont(f), data(d)
{
    maxSegment = maxSeg; // for all mirrors
    drawarea = area;
    createGUI();
    setDirButton(0, true);
    setPatternButton(0,true);
}

PatternGenerator::~PatternGenerator()
{
}

void PatternGenerator::createGUI()
{
    int w = drawarea.getWidth()/2;
    int h = 12;
    //top are the direction
    dirbuttons.clear();
    BUTTON f;
    f.id = 0;
    f.pressed = false;
    f.color = c[0];
    f.name = "FORWARD";
    f.drawarea = ofRectangle(drawarea.getLeft(),drawarea.getTop(),w,h);
    f.fbo.allocate(w, h,GL_RGBA);
    f.fbo.begin();
    ofClear(0,0,0);
    ofSetColor(f.color);
    ofDrawRectRounded(0,0,w,h, 5);
    ofSetColor(255);
    mFont->drawString(f.name, 5, h - 2);
    f.fbo.end();
    dirbuttons.push_back(f);
    
    BUTTON r;
    r.id = 0;
    r.pressed = false;
    r.color = c[0];
    r.name = "REVERSE";
    r.drawarea = ofRectangle(f.drawarea.getRight(),drawarea.getTop(),w,h);
    r.fbo.allocate(w, h,GL_RGBA);
    r.fbo.begin();
    ofClear(0,0,0);
    ofSetColor(r.color);
    ofDrawRectRounded(0,0,w,h, 5);
    ofSetColor(255);
    mFont->drawString(r.name, 5, h - 2);
    r.fbo.end();
    dirbuttons.push_back(r);
    
    w = drawarea.getWidth()/3;
    patternbuttons.clear();
    for (int i = 0; i < PATTERNS::END; i++)
    {
        int x = drawarea.getLeft() + floor(i/5)*w;
        int y = r.drawarea.getBottom() + 10 + (i%5)*h;
        patternbuttons.push_back(BUTTON());
        patternbuttons.back().id = i;
        patternbuttons.back().color = c[0];
        patternbuttons.back().pressed = false;
        patternbuttons.back().name = patternnames[i];
        patternbuttons.back().drawarea = ofRectangle(x,y,w-5,h-2);
        patternbuttons.back().fbo.allocate(w, h,GL_RGBA);
        patternbuttons.back().fbo.begin();
        ofClear(0,0,0);
        ofSetColor(patternbuttons.back().color);
        ofDrawRectRounded(0,0,w,h, 5);
        ofSetColor(255);
        mFont->drawString(patternbuttons.back().name, 5, h - 2);
        patternbuttons.back().fbo.end();
    }
    
}

void PatternGenerator::drawGUI()
{
    ofSetColor(255);
    for (int i = 0; i < dirbuttons.size(); i++)
    {
        dirbuttons[i].fbo.draw(dirbuttons[i].drawarea);
    }
    for (int i = 0; i < patternbuttons.size(); i++)
    {
        patternbuttons[i].fbo.draw(patternbuttons[i].drawarea);
    }
}


void PatternGenerator::updatePattern()
{
    int len = patternOut.size();
    switch (patternID) {
        case PATTERNS::ON_OFF:
        {
            for (int i = 0; i < len; i++)
            {
                patternOut[i] = !patternOut[i];
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
        case PATTERNS::INVERT:
        {
            maxSeqCount = maxSegment;
            for (int j = 0; j < maxSegment; j++)
            {
                patternOut[j] = !patternOut[j];
            }
            break;
        }

        default:
            break;
    }
    
    //based on the direction
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
        case PATTERNS::INVERT:
        {
            maxSeqCount = maxSegment;
            for (int j = 0; j < maxSegment; j++)
            {
                patternOut[j] = !patternOut[j];
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


void PatternGenerator::mousePressed(ofMouseEventArgs & args)
{
    for (int i = 0; i < dirbuttons.size();i++)
    {
        if(dirbuttons[i].drawarea.inside(args.x, args.y))
        {
            setDirButton(i, !dirbuttons[i].pressed);
            return;
        }
    }
    
    for (int i = 0; i < patternbuttons.size();i++)
    {
        
        if(patternbuttons[i].drawarea.inside(args.x, args.y))
        {
            setPatternButton(i, !patternbuttons[i].pressed);
            if(i != lastPressed)
            {
                //he is
            }
            return;
        }
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
    mFont->drawString(dirbuttons[id].name, 5, dirbuttons[id].drawarea.getHeight() - 2);
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
    mFont->drawString(dirbuttons[id].name, 5, dirbuttons[id].drawarea.getHeight() - 2);
    dirbuttons[id].fbo.end();
}

void PatternGenerator::setPatternButton(int id, bool value)
{
    //get the last pressed
    int lastID = -1;
    for (int i = 0; i < patternbuttons.size(); i++)
    {
        if(patternbuttons[i].pressed == true)
        {
            lastID = i;
            break;
        }
    }
    //only update the two
    if(lastID >= 0)
    {
        patternbuttons[lastID].pressed = false;
        patternbuttons[lastID].color = c[false];
        patternbuttons[lastID].fbo.begin();
        ofClear(0,0,0);
        ofSetColor(patternbuttons[lastID].color);
        ofDrawRectRounded(0,0,patternbuttons[lastID].drawarea.getWidth(),patternbuttons[lastID].drawarea.getHeight(), 5);
        ofSetColor(255);
        mFont->drawString(patternbuttons[lastID].name, 5, patternbuttons[lastID].drawarea.getHeight() - 2);
        patternbuttons[lastID].fbo.end();
    }
    patternbuttons[id].pressed = value;
    patternbuttons[id].color = c[true];
    setPattern(id);
    patternbuttons[id].fbo.begin();
    ofClear(0,0,0);
    ofSetColor(patternbuttons[id].color);
    ofDrawRectRounded(0,0,patternbuttons[id].drawarea.getWidth(),patternbuttons[id].drawarea.getHeight(), 5);
    ofSetColor(255);
    mFont->drawString(patternbuttons[id].name, 5, patternbuttons[id].drawarea.getHeight() - 2);
    patternbuttons[id].fbo.end();
}
