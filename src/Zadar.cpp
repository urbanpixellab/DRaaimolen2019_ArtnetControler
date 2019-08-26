//
//  Zadar.cpp
//  Zadar
//
//  Created by Enrico Becker on 30/07/2019.
//
//

#include "Zadar.hpp"


Zadar::Zadar(ofRectangle area, ofTrueTypeFont *f,string name) : drawarea(area), mFont(f),myName(name)
{
    curvePixels.allocate(RES,1,OF_IMAGE_COLOR);
    curveImage.allocate(RES, 1, OF_IMAGE_COLOR);
    
    preview.allocate(area.getWidth()/2,area.getHeight()/2); //top left quadrant
    createCurves();
    dT = 0;
    inverse = false;
    reverse = false;
    addListener();
    createGUI();
    //now init linear
    setCurveButton(CURVES::OFF,true);
}

Zadar::~Zadar()
{
    removeListener();
}

void Zadar::addListener()
{
    ofAddListener(ofEvents().mousePressed, this, &Zadar::mousePressed);
}

void Zadar::removeListener()
{
    ofRemoveListener(ofEvents().mousePressed, this, &Zadar::mousePressed);
}

void Zadar::createGUI()
{
    int w = drawarea.getWidth()/4;
    int h = 12;

    modebuttons.clear();
    BUTTON inv;
    inv.id = 0;
    inv.color = c[0];
    inv.pressed = false;
    inv.name = "INVERT";
    inv.drawarea = ofRectangle(drawarea.getLeft(),drawarea.getCenter().y+2*h,w,h);
    inv.fbo.allocate(w, h,GL_RGBA);
    inv.fbo.begin();
    ofClear(0,0,0);
    ofSetColor(inv.color);
    ofDrawRectRounded(0,0,w,h, 5);
    ofSetColor(255);
    mFont->drawString(inv.name, 5, h-2);
    inv.fbo.end();
    modebuttons.push_back(inv);

    BUTTON rev;
    rev.id = 1;
    rev.color = c[0];
    rev.pressed = false;
    rev.name = "REVERSE";
    rev.drawarea = ofRectangle(inv.drawarea.getRight(),inv.drawarea.getTop(),w,h);
    rev.fbo.allocate(w, h,GL_RGBA);
    rev.fbo.begin();
    ofClear(0,0,0);
    ofSetColor(rev.color);
    ofDrawRectRounded(0,0,w,h, 5);
    ofSetColor(255);
    mFont->drawString(rev.name, 5, h-2);
    rev.fbo.end();
    modebuttons.push_back(rev);
    //w = 80;
    h = 12;

    curvebuttons.clear();
    int maxH = floor(drawarea.getHeight()/20);
    for (int i = 0; i < CURVES::END; i++)
    {
        int x = drawarea.getCenter().x + floor(i/maxH)*w;
        int y = drawarea.getTop() + (i%maxH)*h;
        curvebuttons.push_back(BUTTON());
        curvebuttons.back().id = i;
        curvebuttons.back().color = c[0];
        curvebuttons.back().pressed = false;
        curvebuttons.back().name = curvenames[i];
        curvebuttons.back().drawarea = ofRectangle(x,y,w-5,h-2);
        curvebuttons.back().fbo.allocate(w, h,GL_RGBA);
        curvebuttons.back().fbo.begin();
        ofClear(0,0,0);
        ofSetColor(curvebuttons.back().color);
        ofDrawRectRounded(0,0,w,h, 5);
        ofSetColor(255);
        mFont->drawString(curvebuttons.back().name, 5, h - 2);
        curvebuttons.back().fbo.end();
    }
}

float Zadar::update()
{
    out = 0;
    return out;
}

float Zadar::update(float &dt)
{
    dT = dt;
    if(reverse == true) dT = 1-dT;

    out = curveLUT[curveID][dT*100];

    //cout << out << endl;
    if(inverse == true)
    {
        out = 1.0 - out;
    }
    return out;
}


void Zadar::draw()
{
    ofSetColor(255);
    preview.draw(drawarea.x,drawarea.y);
    ofSetColor(0);
    if(dT != 0)
    {
        float x = drawarea.getLeft() + (preview.getWidth()*dT);
        float y1 = drawarea.getTop();
        float y2 = drawarea.getCenter().y;
        if(reverse == true) x = drawarea.getCenter().x - (preview.getWidth() * (1-dT));
        ofDrawLine(x, y1, x, y2);
    }
    ofSetColor(255);
    drawGUI();
    ofNoFill();
    ofSetColor(255);
    ofDrawRectangle(drawarea.getLeft(),drawarea.getTop(),drawarea.getWidth()/2,drawarea.getHeight()/2);
    ofFill();
}

void Zadar::drawGUI()
{
    mFont->drawString(myName, drawarea.getLeft(), drawarea.getTop() - 10);
    for (int i = 0; i < modebuttons.size(); i++)
    {
        ofSetColor(255);
        modebuttons[i].fbo.draw(modebuttons[i].drawarea);
    }
    for (int i = 0; i < curvebuttons.size(); i++)
    {
        ofSetColor(255);
        curvebuttons[i].fbo.draw(curvebuttons[i].drawarea);
    }
    curveImage.draw(drawarea.getLeft(),drawarea.getCenter().y + 2,drawarea.getWidth()/2,20);
    
}

void Zadar::mousePressed(ofMouseEventArgs & args)
{
    for (int i = 0; i < modebuttons.size();i++)
    {
        if(modebuttons[i].drawarea.inside(args.x, args.y))
        {
            setModeButton(i, !modebuttons[i].pressed);
            return;
        }
    }

    for (int i = 0; i < curvebuttons.size();i++)
    {
        if(curvebuttons[i].drawarea.inside(args.x, args.y))
        {
            setCurveButton(i, !curvebuttons[i].pressed);
            return;
        }
    }
}

void Zadar::setModeButton(int id,bool value)
{
    modebuttons[id].pressed = value;
    modebuttons[id].color = c[modebuttons[id].pressed];
    // now set the state
    if(id == 0) inverse = modebuttons[id].pressed;
    else if(id == 1) reverse = modebuttons[id].pressed;
    modebuttons[id].fbo.begin();
    ofClear(0,0,0);
    ofSetColor(modebuttons[id].color);
    ofDrawRectRounded(0,0,modebuttons[id].drawarea.getWidth(),modebuttons[id].drawarea.getHeight(), 5);
    ofSetColor(255);
    mFont->drawString(modebuttons[id].name, 5, modebuttons[id].drawarea.getHeight() - 2);
    modebuttons[id].fbo.end();
}

void Zadar::setCurveButton(int id, bool value)
{
    //get the last pressed
    int lastID = -1;
    for (int i = 0; i < curvebuttons.size(); i++)
    {
        if(curvebuttons[i].pressed == true)
        {
            lastID = i;
            break;
        }
    }
    //only update the two
    if(lastID >= 0)
    {
        curvebuttons[lastID].pressed = false;
        curvebuttons[lastID].color = c[false];
        curvebuttons[lastID].fbo.begin();
        ofClear(0,0,0);
        ofSetColor(curvebuttons[lastID].color);
        ofDrawRectRounded(0,0,curvebuttons[lastID].drawarea.getWidth(),curvebuttons[lastID].drawarea.getHeight(), 5);
        ofSetColor(255);

        mFont->drawString(curvebuttons[lastID].name, 5, curvebuttons[lastID].drawarea.getHeight() - 2);
        curvebuttons[lastID].fbo.end();
    }
    curvebuttons[id].pressed = value;
    curvebuttons[id].color = c[true];
    setCurve(id);
    curvebuttons[id].fbo.begin();
    ofClear(0,0,0);
    ofSetColor(curvebuttons[id].color);
    ofDrawRectRounded(0,0,curvebuttons[id].drawarea.getWidth(),curvebuttons[id].drawarea.getHeight(), 5);
    ofSetColor(255);
    mFont->drawString(curvebuttons[id].name, 5, curvebuttons[id].drawarea.getHeight() - 2);
    curvebuttons[id].fbo.end();

}

//morphing automated by curve
void Zadar::setCurve(int id)//also creates the preview
{
    ofSetLineWidth(3);
    curveID = id;
    // now draw to fbo
    
    //dont forget to remove
    ofSetColor(255);
    preview.begin();
    ofClear(0,0,0);
    int y = preview.getHeight();
    float iFaktor = preview.getWidth()/float(RES);
    for(int i = 0; i < RES;i++)
    {
        float value = curveLUT[curveID][i];
        ofDrawLine(i*iFaktor, y, i*iFaktor, y - value*y);
        //hier also draw to pixels
        curvePixels.setColor(i,0,ofColor(value*255));
    }
    curveImage.setFromPixels(curvePixels);

    curveImage.update();
    preview.end();
    ofSetLineWidth(1);
}

void Zadar::createCurves()
{
    curveLUT.clear();
    //OFF
    vector<float> Off;
    for(int i = 0;i < 100;i++)
    {
        Off.push_back(0);
    }
    curveLUT.push_back(Off);
    
    //ON
    vector<float> On;
    for(int i = 0;i < 100;i++)
    {
        On.push_back(1);
    }
    curveLUT.push_back(On);
    //linear
    vector<float> Linear;
    for(int i = 0;i < 100;i++)
    {
        Linear.push_back(i/99.);
    }
    curveLUT.push_back(Linear);
    //pwm
    vector<float> Pwm;
    for(int i = 0;i < 100;i++)
    {
        float v = 1;
        if(i >= 50) v = 0;
        Pwm.push_back(v);
    }
    curveLUT.push_back(Pwm);

    //saw
    vector<float> Saw;
    for(int i = 0;i < 100;i++)
    {
        Saw.push_back(1.0 - (i/99.));
    }
    curveLUT.push_back(Saw);

    //triangle
    vector<float> Triangle;
    for(int i = 0;i < 50;i++)
    {
        float y = (i/50.);
        Triangle.push_back(y);
    }
    for(int i = 0;i < 50;i++)
    {
        float y = 1.0 - (i/50.);
        Triangle.push_back(y);
    }
    curveLUT.push_back(Triangle);
    //sine
    vector<float> Sine;
    for(int i = 0;i < 100;i++)
    {
        float y = 0.5 + sin((i/99.)*PI*2)*0.5;
        Sine.push_back(y);
    }
    curveLUT.push_back(Sine);

    //halfsine
    vector<float> Halfsine;
    for(int i = 0;i < 100;i++)
    {
        float y = sin(i/99.*PI);
        Halfsine.push_back(y);
    }
    curveLUT.push_back(Halfsine);
    //adsr
    vector<float> Adsr;
    for(int i = 0;i < 10;i++)
    {
        float y = i/10.;
        Adsr.push_back(y);
    }
    for(int i = 0;i < 20;i++)
    {
        float y = 1.0 - ((i/20.)*0.6);
        Adsr.push_back(y);
    }
    for(int i = 0;i < 50;i++)
    {
        float y = 0.4 - ((i/60.)*0.2);
        Adsr.push_back(y);
    }
    for(int i = 0;i < 20;i++)
    {
        float y = 0.2 - (i/20.)*0.2;
        Adsr.push_back(y);
    }
    curveLUT.push_back(Adsr);

    //easeease
    vector<float> Ease;
    for(int i = 0;i < 100;i++)
    {
        float y = 0.5 + sin((i/99.)*PI - PI * 0.5)*0.5;
        Ease.push_back(y);
    }
    curveLUT.push_back(Ease);

    //random
    vector<float> Rnd;
    float last = 0;
    for(int i = 0;i < 5;i++)
    {
        float val;
        float target = ofRandom(1);
        for(int j = 0;j < 20;j++)
        {
            float dy = target-last;
            val = last+dy*(j/20.);
            Rnd.push_back(val);
        }
        last = val;
    }
    curveLUT.push_back(Rnd);
    //random
    vector<float> Rnd1;
    last = 0;
    for(int i = 0;i < 10;i++)
    {
        float val;
        float target = ofRandom(1);
        for(int j = 0;j < 10;j++)
        {
            float dy = target-last;
            val = last+dy*(j/10.);
            Rnd1.push_back(val);
        }
        last = val;
    }
    curveLUT.push_back(Rnd1);

    //random
    vector<float> Rnd2;
    last = 0;
    for(int i = 0;i < 20;i++)
    {
        float val;
        float target = ofRandom(1);
        for(int j = 0;j < 5;j++)
        {
            float dy = target-last;
            val = last+dy*(j/5.);
            Rnd2.push_back(val);
        }
        last = val;
    }
    curveLUT.push_back(Rnd2);

    //zigzags
    vector<float> ZigZag1;
    last = 0;
    float freq = 4;
    for(int i = 0;i < 100;i++)
    {
        float dy = 0.5+ sin(((i*freq)/100.) * PI * 2)* 0.5;//frequency 4
        float val = i/100. + dy * 0.1;
        ZigZag1.push_back(val);
    }
    curveLUT.push_back(ZigZag1);

    //zigzags
    vector<float> ZigZag2;
    last = 0;
    freq = 8;
    for(int i = 0;i < 100;i++)
    {
        float dy = 0.5+ sin(((i*freq)/100.) * PI * 2)* 0.5;//frequency 4
        float amp = 0.1;
        float shiftFaktor = 1.0 - (i%8)/8.;
        float val = i/100. + (amp*shiftFaktor);
        ZigZag2.push_back(val);
    }
    curveLUT.push_back(ZigZag2);
    vector<float> Stairs;
    last = 0;
    float val = 0;
    for(int i = 0;i < 100;i++)
    {
        if(i%20 == 19) val += 0.25;
        Stairs.push_back(val);
    }
    curveLUT.push_back(Stairs);
    //log
    //exp
    //wobble
    //multi triangle
    //waves
    
    
    //at the end set shift to zero
    //make function which shifts this on
    // like shiftedCurve[100],, shift the array till 100

}
