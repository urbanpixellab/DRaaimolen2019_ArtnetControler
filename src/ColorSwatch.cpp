//
//  ColorSwatch.cpp
//  Sequencer
//
//  Created by Enrico Becker on 06/08/2019.
//

#include "ColorSwatch.hpp"

ColorSwatch::ColorSwatch(ofRectangle draw)
{
    drawarea = draw;
    fbo.allocate(draw.getWidth(),draw.getHeight());
    int xdim = draw.getWidth()/4;
    int ydim = draw.getHeight()/4;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            area[y*4+x] = ofRectangle(drawarea.getLeft()+x*xdim,drawarea.getTop()+y*ydim,xdim,ydim);
        }
    }
    setColor(0);
}
ColorSwatch::~ColorSwatch(){}

void ColorSwatch::draw()
{
    fbo.draw(drawarea);
}

void ColorSwatch::mousePressed(ofMouseEventArgs & args)
{
    if(drawarea.inside(args.x,args.y))
    {
        for (int i = 0; i < 16; i++)
        {
            if(area[i].inside(args.x,args.y))
            {
                setColor(i);
                return;
            }
        }
    }
}

void ColorSwatch::setColor(int id)
{
    colorID = id;
    updateFBO();
}

void ColorSwatch::updateFBO()
{
    fbo.begin();
    ofClear(0,0,0);
    for(int i = 0;i < 16;i++)
    {
        ofSetColor(colors[i]);
        ofDrawRectangle(area[i].x-drawarea.getLeft(),area[i].y-drawarea.getTop(),area[i].width,area[i].height);
    }
    // now highlight the selected color
    ofSetColor(255);
    ofNoFill();
    ofSetLineWidth(2);
    ofDrawRectangle(area[colorID].x-drawarea.getLeft(),area[colorID].y-drawarea.getTop(),area[colorID].width,area[colorID].height);
    ofFill();
    ofSetLineWidth(1);
    fbo.end();
}
