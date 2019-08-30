//
//  RotarySequencer.cpp
//  DraaiLed2019
//
//  Created by Enrico Becker on 28/08/2019.
//
//

#include "RotarySequencer.hpp"

RotarySequencer::RotarySequencer(ofRectangle area,float rad,int count,int id)
{
    myID = id;
    drawarea = area;
    initSteps = count;
    stepID = 0;
    delta = 0;
    hasTrigger = false;
    lastStepTime = 0;
    stepTime = 0.100;

    
    drawFbo.allocate(area.getWidth(),area.getHeight());
    float radius = rad;
    float cx = drawarea.getWidth()/2.;
    float cy = drawarea.getHeight()/2.;
    for (int i = 0; i < initSteps; i++)
    {
        float x = cx + radius * sin(i/float(initSteps)*TWO_PI);
        float y = cy + radius * cos(i/float(initSteps)*TWO_PI);
        Knop data;
        data.position = ofVec2f(x,y);
        data.radius = drawarea.getWidth()/20;
        data.pressed = false;
        data.col = ofColor(0,0,255,128);
        steps.push_back(data);
    }
    delta = 0;
    hasTrigger = false;
    lastStepTime = 0;
    addListener();
    update();
}

RotarySequencer::~RotarySequencer()
{
    removeListener();
}

void RotarySequencer::nextStep()
{
    stepID--;
    //de pends on direction - is forward!!!!
    if(stepID < 0 ) stepID = steps.size() - 1;
    updateFbo();
}

void RotarySequencer::updateFbo()
{
    
    drawFbo.begin();
    ofClear(0,0,0);
    for (int i = 0; i < steps.size(); i++)
    {
        ofSetColor(255, 0, 0,128);
        if(i == stepID) ofSetColor(255, 0, 0,230);
        if(steps[i].pressed) ofSetColor(170, 147, 43,230);
        if(steps[i].pressed && i == stepID)
        {
            ofSetColor(231, 200, 120,255);
            ofNotifyEvent(trigger, myID);
        }
        ofDrawCircle(steps[i].position.x,steps[i].position.y,steps[i].radius);
        ofNoFill();
        ofSetColor(255,255,255,128);
        ofDrawCircle(steps[i].position.x,steps[i].position.y,steps[i].radius);
        ofFill();
    }
    drawFbo.end();
}

void RotarySequencer::update()
{
    float now = ofGetElapsedTimef();
    for (int i = 0; i < steps.size(); i++)
    {
        if(i == stepID)
        {
            steps[i].isStep = true;
            if(steps[i].pressed)
            {
                //search for the next trigger
                int startID = i;
                bool nextTrigger = false;
                int toNextCount = 1;
                int maxStep = 16;
                thisTriggerTime = now;
                while(nextTrigger == false)
                {
                    if(steps[(startID+toNextCount)%maxStep].pressed == true)
                    {
                        nextTrigger = true;
                        break;
                    }
                    else toNextCount++;
                    
                    if(toNextCount > maxStep)
                    {
                        break;
                    }
                }
                deltaTime = stepTime*float(toNextCount+1);//+1 my own length
                nextTriggerTime = thisTriggerTime+deltaTime;
                //cout << "next in " << toNextCount << " delta " << deltaTime << endl;
                //begin with delta
                ofNotifyEvent(trigger, myID);
            }
        }
        else
        {
            steps[i].isStep = false;
            steps[i].drawColorID = 0;
            if(steps[i].pressed) steps[i].drawColorID = 1;
        }
    }

    delta = (now - thisTriggerTime)/(deltaTime);
    //cout << "d " << delta << endl;
    if(delta > 1) delta = 1;
 /*
    // now the other delta stuff
    delta = (now - thisTriggerTime)/(deltaTime);
    //cout << "d " << delta << endl;
    if(delta > 1) delta = 1;
*/
}

float &RotarySequencer::updateDelta()
{
    
    // now the other delta stuff
    float now = ofGetElapsedTimef();
    delta = (now - thisTriggerTime)/(deltaTime);
    //cout << "d " << delta << endl;
    if(delta > 1) delta = 1;
    return delta;
}

void RotarySequencer::resetToBegin()
{
    stepID = 0;
    update();
}


void RotarySequencer::draw()
{
    ofSetColor(255);
    drawFbo.draw(drawarea);
}

void RotarySequencer::mousePressed(ofMouseEventArgs & args)
{
    if (drawarea.inside(args.x, args.y))
    {
        for (int i = 0; i < steps.size(); i++)
        {
            if(ofDist(steps[i].position.x+drawarea.getLeft(),steps[i].position.y+drawarea.getTop(),args.x,args.y) < steps[i].radius)
            {
                steps[i].pressed = !steps[i].pressed;
                break;
            }
        }
        update();
    }
}
