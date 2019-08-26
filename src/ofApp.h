#pragma once

#include "ofMain.h"
#include "ArrayMappingTest.hpp"
#include "PatternEditor.hpp"
#include "ArtnetData.hpp"
#include "GraphicGenerator.hpp"
#include "Mirror.hpp"
#include "UniverseMapper.hpp"

class ofApp : public ofBaseApp{

public:
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

    void setEditorID(int index);
    void setLiveID(int index);
    void isTrigger(int &triggerIndex);
    void exit();

private:
    
    vector<PatternEditor*>  patEditors;
//    PatternEditor           *LIVE;

    vector<ofRectangle>     previewBTNs;
    
    ArtnetData              *artnet;
    ArrayMappingTest    arTest;
    
    ofTrueTypeFont      menueFont;
    int liveSelect,editSelect = 0;
    
    float steplength;
    float timer;
    float seqDelta;
  
    ofFbo                   preview;
    GraphicGenerator        gfx;
  
    vector<Mirror>          mirrors;
    int                     stepcount;//64
    UniverseMapper          *uMapper;
};

// infos
// the sequencer has a static selcetion of segments
// the sequencer the runs through a preprogrammed segment animation
// like lauflicht, an aus....
// 3 step modes playmodes fw rv pingpong
