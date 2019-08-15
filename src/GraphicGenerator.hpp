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
    
    void draw(ofFbo &screen,ofTexture &tex);
    
private:
    ofShader        shader;
    ofMesh          mesh;
};

#endif /* GraphicGenerator_hpp */
