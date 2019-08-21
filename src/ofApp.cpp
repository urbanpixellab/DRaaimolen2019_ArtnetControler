#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    steplength = 0.0625;
    ofSetFrameRate(30);
    timer = 0;
    menueFont.load("verdana.ttf", 8);
    editSelect = 0;
    liveSelect = 0;
    preview.allocate(100, 100);
    
    for(int i = 0;i < 16 ;i++)
    {
        patEditors.push_back(new PatternEditor(PatternEditor(ofRectangle(0,300,800,500),&menueFont)));
        ofAddListener(patEditors.back()->isTrigger, this, &ofApp::isTrigger);
    }
    //LIVE = new PatternEditor();
    
    //testwise preview buttons
    previewBTNs.clear();
    int w = ofGetWidth() / 32;
    for(int i = 0;i < patEditors.size();i++)
    {
        previewBTNs.push_back(ofRectangle(i*2*w,160,w,w));
    }

    //mapping test
    arTest.getOutArray()[0] = 25;
//    cout << "out0 should been 25:" << arTest.getOutArray()[0] << endl;
    arTest.getReverseArray()[4] = 5;
//    cout << "out0 should been 5:" << arTest.getOutArray()[0]<< " " << arTest.getReverseArray()[4] << endl;

    artnet = new ArtnetData();
    patEditors[editSelect]->isVisible(true);
    //LIVE = patEditors[liveSelect];
    
    w = ofGetWidth() / 40;
    int h = 90;//we have max 90 leds in height
    for (int i = 0; i < 20; i++)
    {
        mirrors.push_back(Mirror(i, artnet,ofRectangle(i*w*2,0,w,h)));
    }
//
}

//--------------------------------------------------------------
void ofApp::update()
{
    float now = ofGetElapsedTimef();
    if(now > timer + steplength)
    {
        //set all colors, should been done somewhere else
        for(int i = 0;i < mirrors.size();i++)
        {
            gfx.setColor(patEditors[editSelect]->getColorA(), patEditors[editSelect]->getColorB());
        }

        
        timer = now + steplength;
        //update all
        for (int i = 0; i < patEditors.size(); i++)
        {
            //patEditors[editSelect]->nextStep();
        }
        patEditors[editSelect]->nextStep();

        stepcount++;
        if(stepcount >= 16)
        {
            stepcount = 0;
        }

        //if(editSelect != liveSelect) LIVE->nextStep(); // onl;y if they are not the same update them
    }
    else
    {
        for (int i = 0; i < patEditors.size(); i++)
        {
        //    patEditors[editSelect]->nextStep();
        }
        patEditors[editSelect]->update();
        //LIVE->update();
    }
    // now create the graphic
    gfx.draw(preview,patEditors[editSelect]->getCurve(),patEditors[editSelect]->getDeltaC(),patEditors[editSelect]->getValueA());
    
    // now write to artnet
    
    for(int i = 0;i < mirrors.size();i++)
    {
        mirrors[i].update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    patEditors[editSelect]->drawGUI();
    ofDrawBitmapString("fps " + ofToString(ofGetFrameRate()),0,600);
    int id = 0;
    ofSetColor(255,128,0);
    for(int i = 0;i < previewBTNs.size();i++)
    {
        ofDrawRectangle(previewBTNs[i]);
    }
    ofSetColor(255);
    //preview.draw(0,0,100,100);
    
    for(int i = 0;i < mirrors.size();i++)
    {
        mirrors[i].drawPreview(preview.getTexture());
    }

}

void ofApp::setEditorID(int index)
{
    patEditors[editSelect]->isVisible(false);
    editSelect = index;
    patEditors[editSelect]->isVisible(true);
}

void ofApp::setLiveID(int index)
{
    liveSelect = index;
    //LIVE = patEditors[liveSelect];
}

void ofApp::isTrigger(int &triggerIndex)
{
        if(triggerIndex == 0)
        {
            for(int i = 0;i < mirrors.size();i++)
            {
                bool e = patEditors[editSelect]->getMirrorPattern()[i];
                mirrors[i].setEnables(e,e,e,e);
            }

        }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' ')
    {
        for(int i = 0;i < mirrors.size();i++)
        {
            gfx.setColor(patEditors[editSelect]->getColorA(), patEditors[editSelect]->getColorB());
        }
    }
    if(key == 'n')
    {
//        patCont.setPatternDirection(0);
    }
    if(key == 'm')
    {
//        patCont.setPatternDirection(1);
    }
}

void ofApp::exit()
{
    for(int i = 0;i < patEditors.size();i++)
    {
        delete patEditors[i];
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

    for(int i = 0;i < previewBTNs.size();i++)
    {
        if(previewBTNs[i].inside(x, y))
        {
            setEditorID(i);
        }
    }

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
