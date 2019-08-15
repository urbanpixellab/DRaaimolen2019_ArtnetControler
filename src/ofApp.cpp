#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    steplength = 0.0625;
    timer = 0;
    menueFont.load("verdana.ttf", 8);
    editSelect = 0;
    liveSelect = 0;
    preview.allocate(100, 100);
    
    for(int i = 0;i < 16 ;i++)
    {
        patEditors.push_back(new PatternEditor(PatternEditor(ofRectangle(0,300,800,500),&dataControl,&menueFont)));
    }
    //LIVE = new PatternEditor();
    
    //testwise preview buttons
    previewBTNs.clear();
    int w = ofGetWidth() / 32;
    for(int i = 0;i < patEditors.size();i++)
    {
        previewBTNs.push_back(ofRectangle(i*2*w,0,w,w));
    }
    
    //mapping test
    arTest.getOutArray()[0] = 25;
//    cout << "out0 should been 25:" << arTest.getOutArray()[0] << endl;
    arTest.getReverseArray()[4] = 5;
//    cout << "out0 should been 5:" << arTest.getOutArray()[0]<< " " << arTest.getReverseArray()[4] << endl;
    artnet = new ArtnetData();
    
    patEditors[editSelect]->isVisible(true);
    LIVE = patEditors[liveSelect];

}

//--------------------------------------------------------------
void ofApp::update()
{
    float now = ofGetElapsedTimef();
    if(now > timer + steplength)
    {
        timer = now + steplength;
        //update all
        for (int i = 0; i < patEditors.size(); i++)
        {
            //patEditors[editSelect]->nextStep();
        }
        patEditors[editSelect]->nextStep();
        if(editSelect != liveSelect) LIVE->nextStep(); // onl;y if they are not the same update them
    }
    else
    {
        for (int i = 0; i < patEditors.size(); i++)
        {
        //    patEditors[editSelect]->nextStep();
        }
        patEditors[editSelect]->update();
        LIVE->update();
    }
    // now create the graphic
    gfx.draw(preview,LIVE->getCurve());
    // now write to artnet
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    patEditors[editSelect]->drawGUI();
    ofDrawBitmapString("fps " + ofToString(ofGetFrameRate()),0,600);
    int id = 0;
    ofImage img = artnet->getNode(id).universes[0];
    img.draw(300, 500,150,50);
    ofSetColor(255,128,0);
    for(int i = 0;i < previewBTNs.size();i++)
    {
        ofDrawRectangle(previewBTNs[i]);
    }
    ofSetColor(255);
    preview.draw(0,0,100,100);
    artnet->drawPreview(LIVE->getSegmentPattern());
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
    LIVE = patEditors[liveSelect];
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' ')
    {
//        seqA.resetToBegin();
        
    }
    if(key == 'n')
    {
//        patCont.setPatternDirection(0);
    }
    if(key == 'm')
    {
//        patCont.setPatternDirection(1);
    }
    if(key == '0')
    {
        setEditorID(0);
    }
    if(key == '1')
    {
        setEditorID(1);
    }
    if(key == '2')
    {
        setEditorID(2);
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
