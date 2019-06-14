#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(30);
    for(int i = 0;i < universes;i++)
    {
        ofxArtnetSender *art = new ofxArtnetSender();
        
        artnets.push_back(art);
        //every artnet object consists of an ip and an universe
        art->setup("192.168.12.200",i);
    }

    shader.load("noise.vert", "noise.frag");
    //set ledlengthe per shader as fbo length
    ledStripe.allocate(150, 1,GL_RGB);
    ledStripe.begin();
    ofClear(0,0,0);
    ledStripe.end();
    
    _oscReceiver.setup(7001);
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    receiveOSC(_oscReceiver);
}

void ofApp::receiveOSC(ofxOscReceiver &receiver)
{
    //check for messages
    //and add new addresses to the existing osc addres routing vector
    //which is a struct counting also the amount of calls
    while(receiver.hasWaitingMessages() > 0)
    {
        ofxOscMessage msg;
        receiver.getNextMessage(msg);
        string adr = msg.getAddress();
        bool isNew = true;
        for(int i = 0;i < _oscMessages.size();i++)
        {
            if(_oscMessages[i].getAddress() == adr)
            {
                // we have an existing message
                isNew = false;
                //replace the values
                int numAttr = msg.getNumArgs();
                for (int id = 0; id < numAttr; id++)
                {
                    // check the type
                    _oscMessages[i] = msg;//check if possible
                    
                }
            }
        }
        if(isNew == true)
        {
            //a dd this addres and value
            _oscMessages.push_back(ofxOscMessage(msg));
            cout << "added osc address " << _oscMessages.back().getAddress() <<  " now in total " << _oscMessages.size() << endl;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //create graphics for this stripe with this shader
    
    ofSetColor(255);
    ledStripe.begin();
    ofClear(0,0,0);
    
    shader.begin();
    shader.setUniform1f("width", ledStripe.getWidth());
    shader.setUniform1f("freqR", 1);
    shader.setUniform1f("ampR", 0.55);
    shader.setUniform1f("phaseshiftR", fmod(sin(ofGetElapsedTimef()),0.5));
    // the values are now controlled the offsets
    shader.setUniform1f("freqG", 1);
    shader.setUniform1f("ampG", 1);
    shader.setUniform1f("phaseshiftG", sin(fmod(ofGetElapsedTimef(),3)));
    shader.setUniform1f("freqB", 1);
    shader.setUniform1f("ampB", 1);
    shader.setUniform1f("phaseshiftB", fmod(ofGetElapsedTimef(),4));
    ofDrawRectangle(0, 0, ledStripe.getWidth(),ledStripe.getHeight());
    shader.end();
    ledStripe.end();

    ledStripe.draw(0,0,100,100);
    ofPixels pix;
    ledStripe.readToPixels(pix);
    ofDrawBitmapString(ofToString(pix.getWidth()),10,200);
    ofDrawBitmapString(ofToString(pix.getNumChannels()),10,220);
    ofDrawBitmapString(ofToString(pix.getColor(10)),10,240);
    
    //writeToLedArray(pix,1);
}

void ofApp::writeToLedArray(ofPixels & p,int len)//maybe a mapping from to
{
    //write to all
    for(int i = 0;i < universes;i++)
    {
       artnets[i]->sendArtnet(p);
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
void ofApp::exit()
{
    /*
    for(int i = 0;i < universes;i++)
    {
        delete artnets[i];
    }*/
}
