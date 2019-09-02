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

void GraphicGenerator::drawToFbo(ofFbo &screen,ofTexture &tex,float &delta,float &bright,ofColor &a,ofColor &b)
{
    screen.begin();
    ofClear(0, 0, 0);
    shader.begin();
    shader.setUniform1f("freq",0.25);
    shader.setUniform2f("res",ofVec2f(100,100));
    shader.setUniform1f("bright", bright);
    shader.setUniform1f("shift",100 * delta);//delta time
    shader.setUniform3f("colA", a.r/255.,a.g/255.,a.b/255.);
    shader.setUniform3f("colB", b.r/255.,b.g/255.,b.b/255.);
    shader.setUniformTexture("tex",tex, 0);
    mesh.draw();
    shader.end();
    screen.end();
}

void GraphicGenerator::drawToFbo(ofFbo &screen,ofTexture &tex,float &delta,float &bright,float &master)
{
    screen.begin();
    ofClear(0, 0, 0);
    shader.begin();
    shader.setUniform1f("freq",0.25);
    shader.setUniform2f("res",ofVec2f(100,100));
    shader.setUniform1f("bright", bright*master);
    shader.setUniform1f("shift",100 * delta);//delta time
    shader.setUniform3f("colA", colorA);
    shader.setUniform3f("colB", colorB);
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