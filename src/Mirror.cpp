//
//  Mirror.cpp
//  DraaiLed2019
//
//  Created by Enrico Becker on 15/08/2019.
//
//

#include "Mirror.hpp"

Mirror::Mirror(int id, ArtnetData * artnet,ofRectangle area): artnet(artnet),drawarea(area)
{
    mID = id;
    myNode = floor(id/4);
    //stimmt noch nicht muessen 8 sein nicht 4!!!
    myUniverses[0] = id%4 + 0;
    myUniverses[1] = id%4 + 1;
    //create the drawing function
    mappingMode = MAPPING::CW;
    // no create the draw segments and draw functions
    
    test.allocate(150, 1, 3);
    for (int i = 0; i < 150; i++)
    {
        test.setColor(i, 0, ofColor((i/150.)* 255));//ramp
    }
    testImg.setFromPixels(test);
    int l = 100; // the textur width;

    //4 segments 5 points
    //reverse is in artnet settet up
    //draw onscreen
    ofVec3f a = drawarea.getBottomLeft();
    ofVec3f b = drawarea.getTopLeft();
    ofVec3f c = drawarea.getTopRight();
    ofVec3f d = drawarea.getBottomRight();
    ofVec3f ab = ofVec3f(a.x,drawarea.getCenter().y,0);
    ofVec3f bc = ofVec3f(drawarea.getCenter().x,b.y,0);
    ofVec3f cd = ofVec3f(c.x,drawarea.getCenter().y,0);
    ofVec3f da = ofVec3f(drawarea.getCenter().x,a.y,0);
    ofVec3f end = a;
    
    preview[0].setMode(OF_PRIMITIVE_LINE_STRIP);
    preview[0].addVertex(a);
    preview[0].addVertex(ab);
    preview[0].addVertex(b);

    preview[1].setMode(OF_PRIMITIVE_LINE_STRIP);
    preview[1].addVertex(b);
    preview[1].addVertex(bc);
    preview[1].addVertex(c);

    
    preview[2].setMode(OF_PRIMITIVE_LINE_STRIP);
    preview[2].addVertex(c);
    preview[2].addVertex(cd);
    preview[2].addVertex(d);

    preview[3].setMode(OF_PRIMITIVE_LINE_STRIP);
    preview[3].addVertex(d);
    preview[3].addVertex(da);
    preview[3].addVertex(end);
    
    
    //to pixels not normalized!!!!!
    
    preview[0].addTexCoord(ofVec2f(0,0));
    preview[0].addTexCoord(ofVec2f(0.125*l,0));
    preview[0].addTexCoord(ofVec2f(0.25*l,0));
    
    preview[1].addTexCoord(ofVec2f(0.25*l,0));
    preview[1].addTexCoord(ofVec2f(0.375*l,0));
    preview[1].addTexCoord(ofVec2f(0.5*l,0));
    
    preview[2].addTexCoord(ofVec2f(0.5*l,0));
    preview[2].addTexCoord(ofVec2f(0.625*l,0));
    preview[2].addTexCoord(ofVec2f(0.75*l,0));
    
    preview[3].addTexCoord(ofVec2f(0.75*l,0));
    preview[3].addTexCoord(ofVec2f(0.825*l,0));
    preview[3].addTexCoord(ofVec2f(1.0*l,0));
    


    //preview.addTexCoord(ofVec2f(1,0));//end
    
    all[0].allocate(150, 1);//here i draw allways all segments from one universum
    all[1].allocate(150, 1);//here i draw allways all segments from one universum
    //and draw there the mesh lines with different texture mappings always on 3 points
    //this fo is then readed to pixels and send to artnet universes
    //per loop
    all[0].begin();
    ofClear(0,0,0);
    all[0].end();

    all[1].begin();
    ofClear(0,0,0);
    all[1].end();
    
    //getht bis 120, later with in and out point per segment, and center of IO
    
    
    render[0].setMode(OF_PRIMITIVE_LINE_STRIP);
    render[0].addVertex(ofVec3f(0,0,0));
    render[0].addVertex(ofVec3f(30,0,0));
    render[0].addVertex(ofVec3f(60,0,0));
    
    
    render[1].setMode(OF_PRIMITIVE_LINE_STRIP);
    render[1].addVertex(ofVec3f(90,0,0));
    render[1].addVertex(ofVec3f(120,0,0));
    render[1].addVertex(ofVec3f(150,0,0));

    render[2].setMode(OF_PRIMITIVE_LINE_STRIP);
    render[2].addVertex(ofVec3f(0,0,0));
    render[2].addVertex(ofVec3f(30,0,0));
    render[2].addVertex(ofVec3f(60,0,0));
    
    render[3].setMode(OF_PRIMITIVE_LINE_STRIP);
    render[3].addVertex(ofVec3f(90,0,0));
    render[3].addVertex(ofVec3f(120,0,0));
    render[3].addVertex(ofVec3f(150,0,0));
    
    render[0].addTexCoord(ofVec2f(0*l,0));
    render[0].addTexCoord(ofVec2f(0.125*l,0));
    render[0].addTexCoord(ofVec2f(0.25*l,0));
    
    render[1].addTexCoord(ofVec2f(0.25*l,0));
    render[1].addTexCoord(ofVec2f(0.375*l,0));
    render[1].addTexCoord(ofVec2f(0.5*l,0));
    
    render[2].addTexCoord(ofVec2f(0.5*l,0));
    render[2].addTexCoord(ofVec2f(0.625*l,0));
    render[2].addTexCoord(ofVec2f(0.75*l,0));
    
    
    render[3].addTexCoord(ofVec2f(0.75*l,0));
    render[3].addTexCoord(ofVec2f(0.825*l,0));
    render[3].addTexCoord(ofVec2f(1.0*l,0));


    setTextureMapping(mappingMode);
}

Mirror::~Mirror(){}

void Mirror::setTextureMapping(int mappingID)
{
    //sets on the meshes the texture coordinats to the mapping
    //also a fucked up version
    
}


void Mirror::update()//get shader values and draw the fbo and then to pixels
//or if to heavy calculation get the texture and write it to array
{
    //update draws the graphics to artnet->nodes->universes
    //and the rest getts filled black or stays//// fade out * 0.8!!!!
    //for each segment
    //depending on the mapping
    
    //the length of all segments on decides the framebuffer length to write on
    //the defined pattern
    
    //testwise, i have a pix array with something for the preview and arrays
    
    
    //mesh lines are drawn to big fbo for all segments, the segments are enable always or getten drawn black
    // i have an fbo over the complete length from all segments
    for (int i = 0; i < 2; i++)
    {
        switch (mappingMode)
        {
            case MAPPING::CW:
                //get from artnet the right array and write to it all mirror elements
                //artnet->getNode(myNode).universes[myUniverses[0]].
                //depending on the mapping
                break;
                
            default:
                break;
        }
        //ready to send
        
        // now artnet can send them out
        //but only if they are available!!!
        //artnet->send(myNode, myUniverses[i]);
    }
}

void Mirror::drawPreview(ofTexture &tex)
{
    ofSetLineWidth(5);
    all[0].begin();
    ofClear(0,0,0);
    tex.bind();
    if(enabled[0])render[0].draw();
    if(enabled[1])render[1].draw();
    all[0].end();

    all[1].begin();
    ofClear(0,0,0);
    if(enabled[2])render[2].draw();
    if(enabled[3])render[3].draw();
    all[1].end();
    
    
    //now after everything is writen and sended we can dra a preview to screen
    if(enabled[0])preview[0].draw();
    if(enabled[1])preview[1].draw();
    if(enabled[2])preview[2].draw();
    if(enabled[3])preview[3].draw();
    tex.unbind();
    ofSetLineWidth(1);
    
    all[0].draw(drawarea.getLeft(),drawarea.getBottom()+5,drawarea.getWidth()*2,10);
    all[1].draw(drawarea.getLeft(),drawarea.getBottom()+25,drawarea.getWidth()*2,10);
}

void Mirror::setEnables(bool left,bool top, bool right, bool bottom)
{
    enabled[0] = left;
    enabled[1] = top;
    enabled[2] = right;
    enabled[3] = bottom;
    //calculate the fbo or pixels
}
