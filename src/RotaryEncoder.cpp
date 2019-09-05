//
//  RotaryEncoder.cpp
//
//
//  Created by Enrico Becker on 27/08/2019.
//

#include "RotaryEncoder.hpp"

RotaryEncoder::RotaryEncoder(ofRectangle area,int id,ofTrueTypeFont *f, string name,float min, float max,int sDraw,bool stick,string n[10]):drawarea(area),myID(id),mFont(f),myName(name),stepsDrawn(sDraw),isSticking(stick)
{
    names.clear();
    for(int i = 0;i < sDraw;i++)
    {
        names.push_back(n[i]);
    }
    hasNames = true;
    range = ofVec2f(min,max);
    cCenter.x = drawarea.getWidth()*0.5;
    cRadius = drawarea.getWidth()* 0.3;
    cCenter.y = drawarea.getHeight() - cRadius* 2;
    
    ofRectangle bbox = mFont->getStringBoundingBox(myName, 0, 0);
    fBegin = ofVec2f((drawarea.getWidth()-bbox.getWidth())/2.,bbox.getHeight() + 5);
    
    drawFbo.allocate(drawarea.getWidth(), drawarea.getHeight());
    
    float angle = 0;
    for(int i = 0;i < stepsDrawn + 1;i++)
    {
        angle = (PI*0.75);
        angle += i * (PI * 1.5/float(stepsDrawn));
        float x1 = cCenter.x + cos(angle) * cRadius;
        float y1 = cCenter.y + sin(angle) * cRadius;
        float x2 = cCenter.x + cos(angle) * cRadius*1.3;
        float y2 = cCenter.y + sin(angle) * cRadius*1.3;
        lines.push_back(ofVec4f(x1,y1,x2,y2));
    }
    
    value = 0;
    mapValue = 0;
    updateFbo();
    addListener();
    isActive = false;
}

RotaryEncoder::RotaryEncoder(ofRectangle area,int id,ofTrueTypeFont *f, string name,float min, float max,int sDraw,bool stick):drawarea(area),myID(id),mFont(f),myName(name),stepsDrawn(sDraw),isSticking(stick)
{
    names.clear();
    hasNames = false;
    range = ofVec2f(min,max);
    cCenter.x = drawarea.getWidth()*0.5;
    cRadius = drawarea.getWidth()* 0.3;
    cCenter.y = drawarea.getHeight() - cRadius* 1.3;
    
    ofRectangle bbox = mFont->getStringBoundingBox(myName, 0, 0);
    fBegin = ofVec2f((drawarea.getWidth()-bbox.getWidth())/2.,bbox.getHeight() + 5);
    
    drawFbo.allocate(drawarea.getWidth(), drawarea.getHeight());
    
    float angle = 0;
    for(int i = 0;i < stepsDrawn + 1;i++)
    {
        angle = (PI*0.75);
        angle += i * (PI * 1.5/float(stepsDrawn));
        float x1 = cCenter.x + cos(angle) * cRadius;
        float y1 = cCenter.y + sin(angle) * cRadius;
        float x2 = cCenter.x + cos(angle) * cRadius*1.3;
        float y2 = cCenter.y + sin(angle) * cRadius*1.3;
        lines.push_back(ofVec4f(x1,y1,x2,y2));
    }
    
    value = 0;
    updateFbo();
    addListener();
    isActive = false;
}

RotaryEncoder::~RotaryEncoder(){
    removeListener();
}

void RotaryEncoder::updateFbo()
{
    drawFbo.begin();
    ofClear(0,0,0);
    ofSetColor(0,0,200);
    ofFill();
    ofDrawCircle(cCenter, cRadius);
    ofNoFill();
    ofSetLineWidth(2);
    ofSetColor(255);
    ofDrawCircle(cCenter, cRadius);
    ofFill();
    // the position indicator
    // mapp the value to the angle
    float angle = ofMap(value, 0, 1, PI*0.75, (PI*0.75)+PI*1.5);
    
    float x1 = cCenter.x + cos(angle) * cRadius;
    float y1 = cCenter.y + sin(angle) * cRadius;
    ofDrawLine(cCenter.x,cCenter.y,x1,y1);
    float x2;
    float y2;
    //draw indicators
    for (int i = 0; i < lines.size(); i++)
    {
        ofDrawLine(lines[i].x,lines[i].y,lines[i].z,lines[i].w);
    }
    ofSetLineWidth(1);

    mFont->drawString(myName, fBegin.x,fBegin.y);
    if(hasNames == true)
    {
        mFont->drawString(names[int(mapValue)],fBegin.x+10,drawarea.getHeight()-fBegin.y);
    }
    else{
        mFont->drawString(ofToString(mapValue,2),fBegin.x+10,drawarea.getHeight()-fBegin.y);
    }
    
    drawFbo.end();
}

void RotaryEncoder::setValue(float v)
{

    value = v;
    mapValue = ofMap(value, 0, 1, range.x, range.y);
    updateFbo();
    ofNotifyEvent(newValue,myID);
}

void RotaryEncoder::setRange(ofVec2f r)
{
    range = r;
    mapValue = ofMap(value, 0, 1, range.x, range.y);
    updateFbo();
    ofNotifyEvent(newValue,myID);
}


void RotaryEncoder::draw()
{
    if(!isActive) return;
    drawFbo.draw(drawarea);
}

void RotaryEncoder::mousePressed(ofMouseEventArgs &args)
{
    if(!isActive) return;

    if(drawarea.inside(args.x, args.y))
    {
        clickValue = value;
        mouseStart = ofVec2f(args.x,args.y);
        isSliding = true;

    }
}

void RotaryEncoder::mouseDragged(ofMouseEventArgs &args)
{
    if(!isActive) return;
    if(drawarea.inside(args.x, args.y) && isSliding == true)
    {
        float val;
        float l = (ofVec2f(args.x,args.y) - mouseStart).length()/cRadius;
        if(args.x < mouseStart.x) val = ofClamp(clickValue - l,0,1);
        else val = ofClamp(clickValue + l,0,1);
        if(isSticking)
        {
            val = roundf(val * stepsDrawn) / stepsDrawn;
        }
        
        setValue(val);
    }
}

void RotaryEncoder::mouseReleased(ofMouseEventArgs &args)
{
    if(!isActive) return;
    isSliding = false;
}

void RotaryEncoder::addListener()
{
    ofAddListener(ofEvents().mousePressed, this, &RotaryEncoder::mousePressed);
    ofAddListener(ofEvents().mouseDragged, this, &RotaryEncoder::mouseDragged);
    ofAddListener(ofEvents().mouseReleased, this, &RotaryEncoder::mouseReleased);
}

void RotaryEncoder::removeListener()
{
    ofRemoveListener(ofEvents().mousePressed, this, &RotaryEncoder::mousePressed);
    ofRemoveListener(ofEvents().mouseDragged, this, &RotaryEncoder::mouseDragged);
    ofRemoveListener(ofEvents().mouseReleased, this, &RotaryEncoder::mouseReleased);
}
