//
//  GraphicGenerator.cpp
//  Sequencer
//
//  Created by Enrico Becker on 14/08/2019.
//

#include "GraphicGenerator.hpp"

GraphicGenerator::GraphicGenerator()
{
    shader.load("test.vert","test.frag");
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    mesh.addVertex(ofVec3f(0,0,0));
    mesh.addTexCoord(ofVec2f(0,0));
    mesh.addVertex(ofVec3f(100,0,0));
    mesh.addTexCoord(ofVec2f(100,0));
    mesh.addVertex(ofVec3f(0,100,0));
    mesh.addTexCoord(ofVec2f(0,100));
    mesh.addVertex(ofVec3f(100,100,0));
    mesh.addTexCoord(ofVec2f(100,100));
}

GraphicGenerator::~GraphicGenerator(){}

void GraphicGenerator::drawToFbo(ofFbo &screen,ofTexture &tex,float &delta,float &bright,float &master,float &freq,float &shift,ofColor &ca,ofColor &cb)
{
    screen.begin();
    ofClear(0, 0, 0);
    shader.begin();
    shader.setUniform1f("freq",freq*10);
    shader.setUniform2f("res",ofVec2f(100,100));
    shader.setUniform1f("bright", bright*master);
    shader.setUniform1f("shift",shift *delta * 200);//delta time removed delta
    shader.setUniform3f("colA", ca.r,ca.b,ca.b);
    shader.setUniform3f("colB", cb.r,cb.g,cb.b);
    shader.setUniformTexture("tex",tex, 0);
    mesh.draw();
    shader.end();
    screen.end();
}

void GraphicGenerator::setColor(ofColor a, ofColor b)
{
    colorA.x = (a.r/255.);
    colorA.y = (a.g/255.);
    colorA.z = (a.b/255.);
    
    colorB.x = b.r/255.;
    colorB.y = b.g/255.;
    colorB.z = b.b/255.;
}

void GraphicGenerator::setLiveColor(ofColor a, ofColor b)
{
    colorALive.x = (a.r/255.);
    colorALive.y = (a.g/255.);
    colorALive.z = (a.b/255.);
    
    colorBLive.x = b.r/255.;
    colorBLive.y = b.g/255.;
    colorBLive.z = b.b/255.;
}
