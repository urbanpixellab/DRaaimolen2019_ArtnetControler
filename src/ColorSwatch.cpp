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
    int xdim = draw.getWidth()/9;
    int ydim = draw.getHeight()/5;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            areaA[y*4+x] = ofRectangle(drawarea.getLeft()+x*xdim,ydim+ drawarea.getTop()+y*ydim,xdim,ydim);
        }
    }
    //shift left
    int shift = xdim * 5;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            areaB[y*4+x] = ofRectangle(shift + drawarea.getLeft()+x*xdim,ydim+ drawarea.getTop()+y*ydim,xdim,ydim);
        }
    }
    preview[0] = ofRectangle(drawarea.getLeft(),drawarea.getTop(),xdim*3,ydim);
    preview[1] = ofRectangle(drawarea.getLeft() + 7*xdim,drawarea.getTop(),xdim*3,ydim);
    swap = ofRectangle(drawarea.getLeft()+3*xdim,drawarea.getTop(),xdim*3,ydim);
    
    setColorA(0);
    setColorB(0);
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
            if(areaA[i].inside(args.x,args.y))
            {
                setColorA(i);
                return;
            }
            if(areaB[i].inside(args.x,args.y))
            {
                setColorB(i);
                return;
            }
        }
        if(swap.inside(args.x,args.y)) swapColor();
    }
}

void ColorSwatch::setColorA(int id)
{
    ofNotifyEvent(colorPressed, id);
    colorIDA = id;
    updateFBO();
}

void ColorSwatch::setColorB(int id)
{
    ofNotifyEvent(colorPressed, id);
    colorIDB = id;
    updateFBO();
}

void ColorSwatch::swapColor()
{
    //also think about a link to sequencer toggle
    int tmp = colorIDA;
    colorIDA = colorIDB;
    colorIDB = tmp;
    updateFBO();
    ofNotifyEvent(colorPressed, tmp);
}

void ColorSwatch::updateFBO()
{
    fbo.begin();
    ofClear(0,0,0);
    for(int i = 0;i < 16;i++)
    {
        ofSetColor(colors[i]);
        ofDrawRectangle(areaA[i].x-drawarea.getLeft(),areaA[i].y-drawarea.getTop(),areaA[i].width,areaA[i].height);
        ofDrawRectangle(areaB[i].x-drawarea.getLeft(),areaB[i].y-drawarea.getTop(),areaB[i].width,areaB[i].height);
    }
    ofSetColor(colors[colorIDA]);
    ofDrawRectangle(preview[0].x-drawarea.getLeft(),preview[0].y-drawarea.getTop(),preview[0].width,preview[0].height);
    ofSetColor(colors[colorIDB]);
    ofDrawRectangle(preview[1].x-drawarea.getLeft(),preview[1].y-drawarea.getTop(),preview[1].width,preview[1].height);
    ofSetColor(255);
    ofDrawRectangle(swap.x - drawarea.getLeft(), swap.y - drawarea.getTop(), swap.getWidth(), swap.getHeight());
    ofSetColor(0);
    ofDrawBitmapString("swap", swap.x - drawarea.getLeft(), swap.y - drawarea.getTop());
    fbo.end();
}
