#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(31,27,33);
    steplength = 0.0625;
    ofSetFrameRate(30);
    timer = 0;
    menueFont.load("verdana.ttf", 8);
    editSelect = 0;
    liveSelect = 0;
    preview.allocate(100, 100);
    
    for(int i = 0;i < 16 ;i++)
    {
        patEditors.push_back(new PatternEditor(ofRectangle(0,0,ofGetWidth(),ofGetHeight()),&menueFont));
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
        float x = (ofGetWidth() - w*5) + (i%4*w*1.1);
        float y = w + floor(i/4)*w*1.1;
        previewBTNs.push_back(ofRectangle(x-w/2,y-w/2,w,w));
    }

    artnet = new ArtnetData();
    //LIVE = patEditors[liveSelect];
    
    w = ofGetWidth() / 40;
    int h = 90;//we have max 90 leds in height
    for (int i = 0; i < 20; i++)
    {
        float x = cx + radius*sin((i/20.)*TWO_PI);
        float y = cy + radius*cos((i/20.)*TWO_PI);
        int startUniversum = (i%4)+(i%4);
        cout << startUniversum << endl;
        mirrors.push_back(Mirror(i, artnet,ofRectangle(x-w/2,y-h/2,w,h),startUniversum,&gfx));
    }
    uMapper = new UniverseMapper(ofRectangle(ofGetWidth()/2,ofGetHeight()-100,ofGetWidth()/2,100),150,&menueFont);
    patEditors[editSelect]->setActive(true);
    masterBrightness = new RotaryEncoder(ofRectangle(10,10,100,100), 20, &menueFont, "BRIGHTNESS", 0, 1, 10, false);
    masterBrightness->setActive(true);
    masterClock = 0;
}

//--------------------------------------------------------------
void ofApp::update()
{
    float now = ofGetElapsedTimef();
    if(now >= timer + steplength)
    {
        //first update the gfx
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
            patEditors[i]->nextStep();
        }
        //do all

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
    
    gfx.drawToFbo(preview,patEditors[editSelect]->getCurve(),patEditors[editSelect]->getValueC(),patEditors[editSelect]->getValueA(),masterBrightness->getValue());
    
    // now write to artnet
    
    for(int i = 0;i < mirrors.size();i++)
    {
        //send to artnet
        mirrors[i].update(preview.getTexture());
        int n = floor(i/4);
        
//        artnet->sendTest2(mirrors[i].getPixelsA());
        
        //artnet->send(n,mirrors[i].getUniverseIDA() ,mirrors[i].getPixelsA());
        //artnet->send(n,mirrors[i].getUniverseIDB() ,mirrors[i].getPixelsB());
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
//    ofSetColor(0);
//    ofDrawRectangle(0,0, ofGetWidth(), 150);
    ofSetColor(255);
    patEditors[editSelect]->drawGUI();
    int id = 0;
    for(int i = 0;i < previewBTNs.size();i++)
    {
        ofSetColor(255,128,0);
        ofDrawRectangle(previewBTNs[i]);
        ofSetColor(255);
        menueFont.drawString(ofToString(i+1), previewBTNs[i].getCenter().x,previewBTNs[i].getCenter().y);
    }
    ofSetColor(255);
    
    for(int i = 0;i < mirrors.size();i++)
    {
        mirrors[i].drawPreview(preview.getTexture());
    }
    uMapper->draw();
    
    mirrors[0].getFbo(0).draw(0,0,150,50);
    ofImage img;
    img.setFromPixels(mirrors[0].getPixelsA());
    img.draw(ofGetWidth()/2, ofGetHeight()/2, 150,20);
    masterBrightness->draw();
    ofDrawBitmapString("fps " + ofToString(ofGetFrameRate()),0,20);

}

void ofApp::setEditorID(int index)
{
    patEditors[editSelect]->setActive(false);
    editSelect = index;
    patEditors[editSelect]->setActive(true);
}

void ofApp::setLiveID(int index)
{
    liveSelect = index;
    //LIVE = patEditors[liveSelect];
}

void ofApp::isTrigger(int &triggerIndex)
{
    if(triggerIndex == 0) // the pattern for segments 1= color
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
                cout << "mirror" << i << " : " << left << " " << top << " " << right << " " << bottom << endl;
            }
            else
            {
                mirrors[i].setEnables(false,false,false,false);
                //cout << "mirror" << i << " FALSE " << endl;
            }
        }

    }
    else if(triggerIndex == 1)
    {
        //we have color trigger
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
        //send artnet testwise
        artnet->sendTest();
        cout << "artnet sended" << endl;
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
    delete masterBrightness;
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
