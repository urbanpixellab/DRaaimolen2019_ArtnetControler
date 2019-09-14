//
//  Tap.cpp
//
//
//  Created by Enrico Becker on 10/09/2019.
//

#include "Tap.hpp"

Tap::Tap(ofRectangle area,ofTrueTypeFont *font):drawarea(area),f(font)
{
    tapCount = 0;
    lastPressed = 0;
    ofAddListener(ofEvents().mousePressed, this, &Tap::mousePressed);
}

Tap::~Tap()
{
    ofRemoveListener(ofEvents().mousePressed, this, &Tap::mousePressed);
}

void Tap::draw()
{
    ofSetColor(128,0,0);
    if(pressed == true)
    {
        ofSetColor(255,0,0);
        pressed = false;
    }
    ofDrawRectangle(drawarea);
    ofSetColor(255);
    //ofDrawBitmapString(bpmString, drawarea.getCenter().x, drawarea.getCenter().y);
    f->drawString("TAP", drawarea.getCenter().x, drawarea.getCenter().y-20);
    f->drawString(bpmString, drawarea.getCenter().x, drawarea.getCenter().y);
}

void Tap::mousePressed(ofMouseEventArgs & args)
{
    if(drawarea.inside(args.x, args.y))
    {
        float now = ofGetElapsedTimef();
        pressed = true;
        if(tapCount > 1)
        {
            delta = (now - lastPressed)/4.;
            if(tapCount >= 2)
            {
                tapCount = 0;
                
                ofNotifyEvent(newTimer, delta);
            }
            bpmString = calculateBPM(delta) + " BPM";
            cout << "delta" << delta << endl;
        }
        tapCount++;
        lastPressed = now;
    }
}

string Tap::calculateBPM(float &d)
{
    // limit the range to 25-500bpm or it is a fault
    //60,000 / BPM = one beat in milliseconds
    return ofToString((60 / delta)/4.,1);
}
