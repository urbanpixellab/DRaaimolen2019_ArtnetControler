#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(51,44,53);
    steplength = 0.0625;
    ofSetFrameRate(30);
    timer = 0;
    menueFont.load("verdana.ttf", 8);
    editSelect = 0;
    liveSelect = 0;
    preview.allocate(100, 100);
    
    for(int i = 0;i < 16 ;i++)
    {
        patEditors.push_back(new PatternEditor(ofRectangle(0,0,800,500),&menueFont));
        ofAddListener(patEditors.back()->isTrigger, this, &ofApp::isTrigger);
    }
    //LIVE = new PatternEditor();
    
    //testwise preview buttons
    previewBTNs.clear();
    float cx = ofGetWidth()/2;
    float cy = ofGetHeight()/2;
    int w = ofGetWidth() / 32;
    float radius = (ofGetHeight()/2) * 0.8;
    for(int i = 0;i < patEditors.size();i++)
    {
        float x = cx - 0.6*radius*sin((i/float(patEditors.size()))*TWO_PI+PI);
        float y = cy + 0.6*radius*cos((i/float(patEditors.size()))*TWO_PI+PI);
        previewBTNs.push_back(ofRectangle(x-w/2,y-w/2,w,w));
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
        //mirrors.push_back(Mirror(i, artnet,ofRectangle(i*w*2,0,w,h)));
        float x = cx + radius*sin((i/20.)*TWO_PI);
        float y = cy + radius*cos((i/20.)*TWO_PI);
        mirrors.push_back(Mirror(i, artnet,ofRectangle(x-w/2,y-h/2,w,h)));
    }
    uMapper = new UniverseMapper(ofRectangle(0,ofGetHeight()-100,ofGetWidth(),100),150,&menueFont);

    rotSequencer = new RotarySequencer(ofRectangle((ofGetWidth()-ofGetHeight())/2.0,0,ofGetHeight(),ofGetHeight()),12);
    masterClock = 0;
}

//--------------------------------------------------------------
void ofApp::update()
{
    float now = ofGetElapsedTimef();
    if(now >= timer + steplength)
    {
        masterClock++;
        if (masterClock >= 64) masterClock = 0;
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
        rotSequencer->nextStep();

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
    rotSequencer->update();

}

//--------------------------------------------------------------
void ofApp::draw(){
//    ofSetColor(0);
//    ofDrawRectangle(0,0, ofGetWidth(), 150);
    ofSetColor(255);
    patEditors[editSelect]->drawGUI();
    ofDrawBitmapString("fps " + ofToString(ofGetFrameRate()),0,600);
    int id = 0;
    for(int i = 0;i < previewBTNs.size();i++)
    {
        ofSetColor(255,128,0);
        ofDrawRectangle(previewBTNs[i]);
        ofSetColor(255);
        menueFont.drawString(ofToString(i+1), previewBTNs[i].getCenter().x,previewBTNs[i].getCenter().y);
    }
    ofSetColor(255);
    //preview.draw(0,0,100,100);
    
    for(int i = 0;i < mirrors.size();i++)
    {
        mirrors[i].drawPreview(preview.getTexture());
    }
    uMapper->draw();
    rotSequencer->draw();
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
                if(patEditors[editSelect]->getMirrorPattern()[i] == true)
                {
                    bool left = patEditors[editSelect]->getMirrorSubPattern(i)[0];
                    bool top = patEditors[editSelect]->getMirrorSubPattern(i)[1];
                    bool right = patEditors[editSelect]->getMirrorSubPattern(i)[2];
                    bool bottom = patEditors[editSelect]->getMirrorSubPattern(i)[3];
                    mirrors[i].setEnables(left,top,right,bottom);
                    //cout << "mirror" << i << " : " << left << " " << top << " " << right << " " << bottom << endl;
                }
                else
                {
                    mirrors[i].setEnables(false,false,false,false);
                    //cout << "mirror" << i << " FALSE " << endl;
                }
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
    delete uMapper;
    delete artnet;
    delete rotSequencer;
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
    cout << w << " " << h << endl;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
