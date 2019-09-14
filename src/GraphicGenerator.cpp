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
    previewShader.load("test2.vert","test2.frag");
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
    shader.setUniform3f("colA", ca.r/255.,ca.g/255.,ca.b/255.);
    shader.setUniform3f("colB", cb.r,cb.g,cb.b);
    shader.setUniformTexture("tex",tex, 0);
    mesh.draw();
    shader.end();
    screen.end();
}

void GraphicGenerator::drawToPreviewFbo(ofFbo &screen,ofTexture &tex,float &delta,float &bright,float &master,float &freq,float &shift,ofColor &ca,ofColor &cb)
{
    screen.begin();
    ofClear(0, 0, 0);
    previewShader.begin();
    previewShader.setUniform1f("freq",freq*10);
    previewShader.setUniform2f("res",ofVec2f(100,100));
    previewShader.setUniform1f("bright", bright*master);
    previewShader.setUniform1f("shift",shift *delta * 200);//delta time removed delta
    previewShader.setUniform3f("colA", ca.r/255.,ca.g/255.,ca.b/255.);
    previewShader.setUniform3f("colB", cb.r,cb.g,cb.b);
    previewShader.setUniformTexture("tex",tex, 0);
    mesh.draw();
    previewShader.end();
    screen.end();
}

void GraphicGenerator::reloadShader()
{
    shader.load("test.vert","test.frag");
    previewShader.load("test2.vert","test2.frag");
}