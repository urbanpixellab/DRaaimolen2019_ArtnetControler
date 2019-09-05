//
//  RotarySequencer.cpp
//  DraaiLed2019
//
//  Created by Enrico Becker on 28/08/2019.
//
//

#include "RotarySequencer.hpp"

RotarySequencer::RotarySequencer(ofRectangle area,float rad,int count,int id,ofColor color)
{
    baseColor = color;
    myID = id;
    drawarea = area;
    initSteps = count;
    stepID = 0;
    delta = 0;
    hasTrigger = false;
    lastStepTime = 0;
    stepTime = 0.0625;

    
    drawFbo.allocate(area.getWidth(),area.getHeight());
    float radius = rad;
    float cx = drawarea.getWidth()/2.;
    float cy = drawarea.getHeight()/2.;
    for (int i = 0; i < initSteps; i++)
    {
        float x = cx + radius * cos(i/float(initSteps)*TWO_PI + PI*1.5);
        float y = cy + radius * sin(i/float(initSteps)*TWO_PI + PI*1.5);
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
    isActive = false;;
}

RotarySequencer::~RotarySequencer()
{
    removeListener();
}

void RotarySequencer::nextStep()
{
    float now = ofGetElapsedTimef();

    stepID++;
    //de pends on direction - is forward!!!!
    if(stepID >= 16 ) stepID = 0;
    updateFbo();
    stepTime = now - lastStepTime;
    lastStepTime = now;
//    cout << "steptime mus been checked " << stepTime << endl;

    //do delta calculation if we have a hit on the sequencer
    if(steps[stepID].pressed == true)
    {
        int startID = stepID;
        bool nextTrigger = false;
        int toNextCount = 1;
        int maxStep = 16;
        thisTriggerTime = now;
        while(nextTrigger == false || toNextCount > maxStep)
        {
            if(steps[(startID+toNextCount)%maxStep].pressed == true)
            {
                nextTrigger = true;
            }
            else toNextCount++;
            
        }
        deltaTime = stepTime*float(toNextCount+1);//+1 my own length
        nextTriggerTime = thisTriggerTime+deltaTime;
        if(isActive) ofNotifyEvent(trigger, myID);
    }
}


void RotarySequencer::updateFbo()
{
    
    drawFbo.begin();
    ofClear(0,0,0);
    for (int i = 0; i < steps.size(); i++)
    {
        ofSetColor(baseColor,128);
        if(i == stepID) ofSetColor(baseColor,230);
        if(steps[i].pressed) ofSetColor(baseColor.r*(170/255.), baseColor.g*(147/255.), baseColor.b*(43/255.),230);
        if(steps[i].pressed && i == stepID)
        {
            ofSetColor(baseColor.r*(231/255.), baseColor.g*(200/255.), baseColor.b*(120/255.),255);
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
    if(isActive == false) return;

    float now = ofGetElapsedTimef();
    delta = (now - thisTriggerTime)/(deltaTime);
    if(delta > 1.0) delta = 1;
//    cout << "d " << delta << endl;
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
    if(isActive == false) return;
    ofSetColor(255);
//    cout << "d " << delta << endl;
    drawFbo.draw(drawarea);
}

void RotarySequencer::mousePressed(ofMouseEventArgs & args)
{
    if(isActive == false) return;
    if (drawarea.inside(args.x, args.y))
    {
        for (int i = 0; i < steps.size(); i++)
        {
            if(ofDist(steps[i].position.x+drawarea.getLeft(),steps[i].position.y+drawarea.getTop(),args.x,args.y) < steps[i].radius)
            {
                steps[i].pressed = !steps[i].pressed;
                cout << "pressed step" << i << endl;
                break;
            }
        }
        update();
    }
}
