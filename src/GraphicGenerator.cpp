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

void GraphicGenerator::draw(ofFbo &screen,ofTexture &tex)
{
    float brightness  = 1;
    screen.begin();
    ofClear(0, 0, 0);
    shader.begin();
    shader.setUniform2f("res",ofVec2f(100,100));
    shader.setUniform1f("shift",100* (0.5 + sin(ofGetElapsedTimef())*0.5));
    shader.setUniform3f("colA", ofVec3f(1,0,0.7));
    shader.setUniform3f("colB", ofVec3f(0,1,0.5));
    shader.setUniformTexture("tex",tex, 0);
    mesh.draw();
    shader.end();
    screen.end();
}
