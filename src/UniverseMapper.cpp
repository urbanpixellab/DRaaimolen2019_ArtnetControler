//
//  UniverseMapper.cpp
//  DraaiLed2019
//
//  Created by Enrico Becker on 22/08/2019.
//
//

#include "UniverseMapper.hpp"

UniverseMapper::UniverseMapper(ofRectangle area,int length,ofTrueTypeFont *f):drawarea(area),mFont(f)
{
    ofFbo singleButton;
    int perRow = 30;
    int w = (ofGetWidth()-100)/perRow;
    int h = drawarea.getHeight()/ceil(length/perRow);
    singleButton.allocate(w,h);
    singleButton.begin();
    ofNoFill();
    ofClear(0,0,0);
    ofSetColor(255);
    ofSetLineWidth(1);
    ofDrawRectangle(0, 0,w, singleButton.getHeight());
    ofFill();
    singleButton.end();
    buttonOverlay.allocate(drawarea.getWidth(),drawarea.getHeight());
    buttonOverlay.begin();
    ofClear(0,0,0);
    for (int i = 0; i < length; i++)
    {
        int x = (i%perRow)*w;
        int y = floor(i/perRow)*h;
        singleButton.draw(x,y);
        areas.push_back(ofRectangle(x+drawarea.getLeft(),y+drawarea.getTop(),w,h));
        y += h;
        mFont->drawString(ofToString(i), x, y);
    }
    buttonOverlay.end();

    for (int i = 0; i < 2; i++)
    {
        mirSeg[i].pixelIDs[0] = floor(150*(0.1+0.5*i));
        mirSeg[i].pixelIDs[1] = floor(150*(0.25+0.5*i));
        mirSeg[i].pixelIDs[2] = floor(150*(0.4+0.5*i));
        mirSeg[i].isInverse = false;
        mirSeg[i].lengthPixels = abs(mirSeg[i].pixelIDs[2] - mirSeg[i].pixelIDs[0]);
        mirSeg[i].texCoords[0] = ofVec2f(0,0);
        mirSeg[i].texCoords[1] = ofVec2f(0.5,0);
        mirSeg[i].texCoords[2] = ofVec2f(1,0);
        for (int s = 0; s < mirSeg[i].lengthPixels; s++)
        {
            mirSeg[i].pixels.push_back(mirSeg[i].pixelIDs[0]+s);
        }
    }
    
    drawFbo.allocate(drawarea.getWidth(),drawarea.getHeight());
    updateFbo();
    ofAddListener(ofEvents().mousePressed, this, &UniverseMapper::mousePressed);
    ofAddListener(ofEvents().mouseDragged, this, &UniverseMapper::mouseDragged);
    ofAddListener(ofEvents().mouseReleased, this, &UniverseMapper::mouseReleased);
    
}
UniverseMapper::~UniverseMapper()
{
    ofRemoveListener(ofEvents().mousePressed, this, &UniverseMapper::mousePressed);
    ofRemoveListener(ofEvents().mouseDragged, this, &UniverseMapper::mouseDragged);
    ofRemoveListener(ofEvents().mouseReleased, this, &UniverseMapper::mouseReleased);
}

void UniverseMapper::updateFbo()
{
    drawFbo.begin();
    ofClear(0,0,0);
    ofSetColor(128);
    ofDrawRectangle(0,0,drawFbo.getWidth(),drawFbo.getHeight());
    ofPushMatrix();
    ofTranslate(-drawarea.getTopLeft());
    ofSetColor(0,0,160,128);
    for (int i = 0; i < mirSeg[0].pixels.size(); i++)
    {
        ofDrawRectangle(areas[mirSeg[0].pixels[i]]);
    }
    for (int i = 0; i < mirSeg[1].pixels.size(); i++)
    {
        ofDrawRectangle(areas[mirSeg[1].pixels[i]]);
    }
    
    ofPopMatrix();
    ofSetColor(255);
    buttonOverlay.draw(0,0);
    drawFbo.end();
}

void UniverseMapper::updateSegment(int id,int beginPixel, int endPixel)
{
    mirSeg[id].pixelIDs[0] = beginPixel;
    mirSeg[id].pixelIDs[2] = endPixel;
    mirSeg[id].pixelIDs[1] = (beginPixel+endPixel)/2;
    mirSeg[id].isInverse = false;
    mirSeg[id].lengthPixels = abs(mirSeg[id].pixelIDs[2] - mirSeg[id].pixelIDs[0]);
    mirSeg[id].texCoords[0] = ofVec2f(0,0);//shal been calulated and also been adjusted
    mirSeg[id].texCoords[1] = ofVec2f(0.5,0);
    mirSeg[id].texCoords[2] = ofVec2f(1,0);
    mirSeg[id].pixels.clear();
    for (int s = 0; s < mirSeg[id].lengthPixels; s++)
    {
        mirSeg[id].pixels.push_back(mirSeg[id].pixelIDs[0]+s);
    }
    updateFbo();
}

void UniverseMapper::draw()
{
    ofSetColor(255);
    drawFbo.draw(drawarea);
}

void UniverseMapper::mousePressed(ofMouseEventArgs & args)
{
    //first give me the distance
    if(drawarea.inside(args.x, args.y) == false) return;
    //now the closest ion or out point select
    //first segment
    ioPixelSelect = -1;//we dont have an input
    if(areas[mirSeg[0].pixelIDs[0]].inside(args.x,args.y)) ioPixelSelect = 0;
    else if(areas[mirSeg[0].pixelIDs[2]-1].inside(args.x,args.y)) ioPixelSelect = 1;
    else if(areas[mirSeg[1].pixelIDs[0]].inside(args.x,args.y)) ioPixelSelect = 2;
    else if(areas[mirSeg[1].pixelIDs[2]-1].inside(args.x,args.y)) ioPixelSelect = 3;
    
    //which one is the shortest, is my select of action by position
    // if i am close to an in or outpoint the select this one and give notification for dragging this point by
    //updating the fbo which is drawn on the new positions
    cout << "pressed id " << ioPixelSelect << endl;
}

void UniverseMapper::mouseDragged(ofMouseEventArgs & args)
{
    if(drawarea.inside(args.x, args.y) == false || ioPixelSelect < 0)
    {
        ioPixelSelect = -1;
        return;
    }
    //which area i am in
    //new begin
    int s = -1;
    for (int i = 0; i < areas.size(); i++)
    {
        if(areas[i].inside(args.x,args.y))
        {
            s = i;
            break;
        }
    }
    if(s < 0) return;
    int sID = floor(ioPixelSelect/2);
    int begin = mirSeg[sID].pixelIDs[0];
    int end = mirSeg[sID].pixelIDs[2];
    if(ioPixelSelect%2 == 0) //begin
    {
        begin = s;
    }
    else
    {
        end = s;
    }
    updateSegment(ioPixelSelect/2, begin, end);
    updateFbo();
}

void UniverseMapper::mouseReleased(ofMouseEventArgs & args)
{
    if(drawarea.inside(args.x, args.y) == false || ioPixelSelect < 0)
    {
        ioPixelSelect = -1;
        return;
    }
    //which area i am in
    //new begin
    int s = -1;
    for (int i = 0; i < areas.size(); i++)
    {
        if(areas[i].inside(args.x,args.y))
        {
            s = i;
            break;
        }
    }
    if(s < 0) return;
    int sID = floor(ioPixelSelect/2);
    int begin = mirSeg[sID].pixelIDs[0];
    int end = mirSeg[sID].pixelIDs[2];
    if(ioPixelSelect%2 == 0) //begin
    {
        begin = s;
    }
    else
    {
        end = s;
    }
    updateSegment(ioPixelSelect/2, begin, end);
    updateFbo();
}
