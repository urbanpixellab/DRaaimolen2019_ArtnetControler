#pragma once

#include "ofMain.h"
#include "ofxArtnet.h"
#include "ofxOsc.h"

#define universes 8

class ofApp : public ofBaseApp{

public:
    struct UNIVERSE
    {
        //holds the outgoing
        int outArray[450];// exactly for our purpose
        
    };
    
    struct ADDRESS
    {
        string path;
        int counter;
        int i_Value;
        float f_Value;
    };
    
    struct LEDSEGMENT
    {
        ofFbo fbo;
        //the states
        bool reverse; //to shgader
        int length; //to shader
        int artnetnode;// or ip
    };
    
    struct STEP
    {
        //for the stepsequencer, holds all settings
        //for drawing
        int color[2] = {0,255}; // color in hsb, only one value as fader red to red
        bool brightness;// zeichen mich, ja nein
        int frequenz;
        int curveSelect;
        int offset2PI;
        
        ofRectangle drawarea;
        
    };
    
    void setup();
    void createStepSequencer();
    void updateStepSequencer(int steps = 16);
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void writeToLedArray(ofPixels & p);
    void exit();

    void receiveOSC(ofxOscReceiver &receiver);
    
    
private:
    ofShader    shader;
    ofFbo       ledStripe;
    vector<ofxArtnetSender*> artnets;

    vector<ofApp::STEP> step;

    
    ofxOscReceiver  _oscReceiver;
    vector<ofxOscMessage> _oscMessages;//damit sind die immer aktuell abrufbar!!! und die werte sind ueber pointer verlinkbar zu machen
};
