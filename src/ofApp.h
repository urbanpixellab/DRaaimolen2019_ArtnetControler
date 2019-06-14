#pragma once

#include "ofMain.h"
#include "ofxArtnet.h"
#include "ofxOsc.h"

#define universes 8

class ofApp : public ofBaseApp{

public:
    struct ADDRESS
    {
        string path;
        int counter;
        int i_Value;
        float f_Value;
    };
    
    
    void setup();
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

    void writeToLedArray(ofPixels & p, int length);
    void exit();

    void receiveOSC(ofxOscReceiver &receiver);
    
    
private:
    ofShader    shader;
    ofFbo       ledStripe;
    vector<ofxArtnetSender*> artnets;

    
    ofxOscReceiver  _oscReceiver;
    vector<ofxOscMessage> _oscMessages;//damit sind die immer aktuell abrufbar!!! und die werte sind ueber pointer verlinkbar zu machen
};
