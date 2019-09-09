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
    
    void drawToFbo(ofFbo &screen,ofTexture &tex, float &delta,float &bright,float &master,float &freq,float &shift,ofColor &ca,ofColor &cb);
private:
    ofShader        shader;
    ofMesh          mesh;
};

#endif /* GraphicGenerator_hpp */
