//
//  GraphicGenerator.hpp
//  Sequencer
//
//  Created by Enrico Becker on 14/08/2019.
//

#ifndef GraphicGenerator_hpp
#define GraphicGenerator_hpp

#include <stdio.h>
#include "ofMain.h"

class GraphicGenerator
{
public:
    GraphicGenerator();
    ~GraphicGenerator();
    
    void drawToFbo(ofFbo &screen,ofTexture &tex, float &delta,float &bright,float &master,float &freq,float &shift);
    void drawToFboPreview(ofFbo &screen,ofTexture &tex, float &delta,float &bright,float &master,float &freq,float &shift);
    void setColor(ofColor a, ofColor b);
    void setLiveColor(ofColor a, ofColor b);
    
private:
    ofShader        shader;
    ofMesh          mesh;
    ofVec3f         colorA;
    ofVec3f         colorB;
    ofVec3f         colorALive;
    ofVec3f         colorBLive;
};

#endif /* GraphicGenerator_hpp */
