//
//  RotarySequencer.cpp
//  DraaiLed2019
//
//  Created by Enrico Becker on 28/08/2019.
//
//

#include "RotarySequencer.hpp"

RotarySequencer::RotarySequencer(ofRectangle area,int count)
{
    drawarea = area;
    initSteps = count;
    stepID = 0;
    
    drawFbo.allocate(area.getWidth(),area.getHeight());
    float radius = (drawarea.getWidth()/2)*0.65;
    float cx = drawarea.getWidth()/2.;
    float cy = drawarea.getHeight()/2.;
    for (int i = 0; i < initSteps; i++)
    {
        float x = cx + radius * sin(i/float(initSteps)*TWO_PI);
        float y = cy + radius * cos(i/float(initSteps)*TWO_PI);
        Knop data;
        data.position = ofVec2f(x,y);
        data.radius = 16;
        data.pressed = false;
        data.col = ofColor(0,0,255,128);
        steps.push_back(data);
    }
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
    update();
}

void RotarySequencer::update()
{
    drawFbo.begin();
    ofClear(0,0,0);
    for (int i = 0; i < steps.size(); i++)
    {
        ofSetColor(255, 0, 0,128);
        if(i == stepID) ofSetColor(255, 0, 0,230);
        if(steps[i].pressed) ofSetColor(170, 147, 43,230);
        if(steps[i].pressed && i == stepID) ofSetColor(231, 200, 120,255);
        ofDrawCircle(steps[i].position.x,steps[i].position.y,steps[i].radius);
        ofNoFill();
        ofSetColor(255,255,255,128);
        ofDrawCircle(steps[i].position.x,steps[i].position.y,steps[i].radius);
        ofFill();
    }
    drawFbo.end();
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
            if(ofDist(steps[i].position.x+drawarea.getLeft(),steps[i].position.y,args.x,args.y) < steps[i].radius)
            {
                steps[i].pressed = !steps[i].pressed;
            }
        }
        update();
    }
}
