//
//  UniverseControler.cpp
//  DRaaimolen2019_ArtnetControler
//
//  Created by Enrico Becker on 05/09/2019.
//

#include "UniverseControler.hpp"

UniverseControler::UniverseControler(ofRectangle area,ofTrueTypeFont *f): drawarea(area),mFont(f)
{
    isActive = false;
    uBtn.clear();
    float bW = drawarea.getWidth()/20;
    float bH = drawarea.getHeight()*0.125;
    float x = drawarea.getLeft();
    float y = drawarea.getTop();
    ofRectangle pA = ofRectangle(drawarea.x,drawarea.y+drawarea.getHeight()*0.25,drawarea.getWidth(),drawarea.getHeight()*0.75);
    for (int i = 0; i < 40;i++)
    {
        UniverseMapper *u = new UniverseMapper(pA,i,150,mFont);
        universes.push_back(u);
        uBtn.push_back(ofRectangle(x+(i%20)*bW,y+(i/20)*bH,bW,bH));
    }
    editUniverse(0);
    ofAddListener(ofEvents().mousePressed, this, &UniverseControler::mousePressed);
}

UniverseControler::~UniverseControler()
{
    ofRemoveListener(ofEvents().mousePressed, this, &UniverseControler::mousePressed);
    for (int i = 0; i < universes.size();i++)
    {
        delete universes[i];
    }
}

void UniverseControler::editUniverse(int id)
{
    uSelect = id;
    for(int i = 0;i < universes.size();i++)
    {
        if(i == uSelect) universes[uSelect]->setActive(true);
        else universes[i]->setActive(false);
    }
}


void UniverseControler::draw()
{
    if(!isActive) return;
    universes[uSelect]->draw();
    //drawButtons
    for(int i = 0;i < uBtn.size();i++)
    {
        ofSetColor(27,0,0);
        ofDrawRectangle(uBtn[i]);
        ofSetColor(255);
        mFont->drawString(ofToString(i), uBtn[i].getLeft(), uBtn[i].getCenter().y);
        if(i == uSelect)
        {
            ofNoFill();
            ofSetColor(255,0,0);
            ofDrawRectangle(uBtn[i]);
            ofFill();
        }
    }
    ofSetColor(255);
}

void UniverseControler::mousePressed(ofMouseEventArgs &args)
{
    if(!isActive) return;
    for(int i = 0;i < uBtn.size();i++)
    {
        if(uBtn[i].inside(args.x,args.y))
        {
            editUniverse(i);
        }
    }
}
