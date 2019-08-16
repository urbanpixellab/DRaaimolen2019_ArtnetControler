//
//  StepSequencer.cpp
//  Sequencer
//
//  Created by Enrico Becker on 05/08/2019.
//

#include "StepSequencer.hpp"


StepSequencer::StepSequencer(ofRectangle drawarea,int maxStep,int id): myID(id)
{
    createSequencer(drawarea,maxStep);
    delta = 0;
    hasTrigger = false;
    lastStepTime = 0;
}

StepSequencer::~StepSequencer()
{
    
}

void StepSequencer::createSequencer(ofRectangle drawarea,int maxStep)
{
    
    area = drawarea;
    int spacer = 5;
    steps.clear();
    triggerSets.clear();
    int w = drawarea.getWidth()/maxStep;
    int h = drawarea.getHeight()*0.5;
    for (int i = 0; i < maxStep; i++)
    {
        STEP S;
        S.drawarea = ofRectangle(drawarea.getLeft() + (i*w),drawarea.getTop(),w - spacer,h);
        S.pressed = false;
        S.isStep = false;
        S.drawColorID = 0;
        steps.push_back(S);
        triggerSets.push_back(false);
    }
    
    // create all fbos
    buttons[0].allocate(w, h);
    buttons[0].begin();
    ofClear(0, 0, 0);
    ofSetColor(55);
    ofDrawRectRounded(0, 0, w, h,5);
    buttons[0].end();
    
    buttons[1].allocate(w, h);
    buttons[1].begin();
    ofClear(0, 0, 0);
    ofSetColor(255,0,0);
    ofDrawRectRounded(0, 0, w, h,5);
    buttons[1].end();

    radius = (w / 2) - 2;

    update();
}

void StepSequencer::nextStep()
{
    stepID++;
    float now = ofGetElapsedTimef();
    if(stepID >= steps.size()) stepID = 0;
    //calculate the step time;
    stepTime = now - lastStepTime;
    lastStepTime = now;
    update();
}

void StepSequencer::update()
{
    // calculate the delta and update the sequencer only when needed
    // draw the sequncer by fbos with the corresponding colors which are created earlier,
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
}

float &StepSequencer::updateDelta()
{
    
    // now the other delta stuff
    float now = ofGetElapsedTimef();
    delta = (now - thisTriggerTime)/(deltaTime);
    //cout << "d " << delta << endl;
    if(delta > 1) delta = 1;
    return delta;
}

void StepSequencer::resetToBegin()
{
    stepID = 0;
    update();
}

void StepSequencer::drawSequencer()
{
    ofSetColor(255);
    for (int i = 0; i < steps.size(); i++)
    {
        buttons[steps[i].drawColorID].draw(steps[i].drawarea);
    }
    //now draw the stepper index
    ofSetColor(color[2]);
    ofDrawCircle(steps[stepID].drawarea.getCenter().x,steps[stepID].drawarea.getBottom() - radius, radius);
}

void StepSequencer::mousePressed(ofMouseEventArgs & args)
{
    if (area.inside(args.x, args.y))
    {
        for (int i = 0; i < steps.size(); i++)
        {
            if(steps[i].drawarea.inside(args.x,args.y))
            {
                steps[i].pressed = !steps[i].pressed;
            }
        }
    }
}

vector<bool> &StepSequencer::getSteps()
{
    for(int i = 0;i < steps.size();i++)
    {
        triggerSets[i] = steps[i].pressed;
    }
    return triggerSets;
}
