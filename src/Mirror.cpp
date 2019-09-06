//
//  Mirror.cpp
//  DraaiLed2019
//
//  Created by Enrico Becker on 15/08/2019.
//
//

#include "Mirror.hpp"

Mirror::Mirror(int id, ArtnetData * artnet,ofRectangle area,int startUniversum,GraphicGenerator *g): artnet(artnet),drawarea(area),gfx(g)
{
    mID = id;
    myNode = floor(id/4);
    //stimmt noch nicht muessen 8 sein nicht 4!!!
    myUniverses[0] = startUniversum;
    myUniverses[1] = startUniversum + 1;
//    cout << "mirror id " << mID << " node id " << myNode << " my universes " << myUniverses[0] << " " << myUniverses[1] << endl;
    //create the drawing function
    mappingMode = MAPPING::CW;
    // no create the draw segments and draw functions
    
    outPixelsA.allocate(150, 1, 3);
    outPixelsB.allocate(150, 1, 3);
    allBlack.allocate(150, 1, 3);
    whitePix.allocate(150,1,3);
    for (int i = 0; i < 150; i++)
    {
        //ofColor c = ofColor((i/150.)* 255);
        ofColor c = ofColor(ofRandom(255),ofRandom(255),ofRandom(255));
        outPixelsA.setColor(i, 0, c);//ramp
        outPixelsB.setColor(i, 0, c);//ramp
        allBlack.setColor(i, 0, ofColor(0));//ramp
        whitePix.setColor(i, 0, ofColor(255));//ramp
    }
    
    outImgA.setFromPixels(outPixelsA);
    outImgB.setFromPixels(outPixelsA);

    createUniverses();
    
    setTextureMapping(mappingMode);
}

Mirror::~Mirror(){}

void Mirror::createUniverses()
{
    int l = 100; // the textur width;
    //preview cersion cw
    
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
    preview[0].addTexCoord(ofVec2f(0.2*l,0));
    preview[0].addTexCoord(ofVec2f(0.4*l,0));
    
    preview[1].addTexCoord(ofVec2f(0.4*l,0));
    preview[1].addTexCoord(ofVec2f(0.45*l,0));
    preview[1].addTexCoord(ofVec2f(0.5*l,0));
    
    preview[2].addTexCoord(ofVec2f(0.5*l,0));
    preview[2].addTexCoord(ofVec2f(0.625*l,0));
    preview[2].addTexCoord(ofVec2f(0.75*l,0));
    
    preview[3].addTexCoord(ofVec2f(0.75*l,0));
    preview[3].addTexCoord(ofVec2f(0.825*l,0));
    preview[3].addTexCoord(ofVec2f(1.0*l,0));
    
    
    
    //preview.addTexCoord(ofVec2f(1,0));//end
    
    all[0].allocate(150, 1,GL_RGB);//here i draw allways all segments from one universum
    all[1].allocate(150, 1,GL_RGB);//here i draw allways all segments from one universum
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
    //draw render linear for mapping
    
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

}

void Mirror::setUniverses(int &l1, int &l2, int &t1, int &t2,int &r1, int &r2, int &b1, int &b2)
{
    //left top right bottom and cw
    //a1 tx = 0
    //b2 tx = 1
    // check the order
    // generate the texcoords
    // set the render meshes
    // set the render end preview textcoords to cw for begin
    // now the texcoords
    int l = 100;// the length
    
    render[0].getVertices()[0].x = l1;
    render[0].getVertices()[2].x = l2;
    render[0].getVertices()[1].x = (l1+l2)/2;

    render[0].getTexCoords()[0].x = 0*l;
    render[0].getTexCoords()[2].x = 0.5*l;
    render[0].getTexCoords()[1].x = 0.25*l;
    
    render[1].getVertices()[0].x = t1;
    render[1].getVertices()[2].x = t2;
    render[1].getVertices()[1].x = (t1+t2)/2;

    render[1].getTexCoords()[0].x = 0.5*l;
    render[1].getTexCoords()[2].x = 1*l;
    render[1].getTexCoords()[1].x = 0.75*l;

    render[2].getVertices()[0].x = r1;
    render[2].getVertices()[2].x = r2;
    render[2].getVertices()[1].x = (r1+r2)/2;
    
    render[2].getTexCoords()[0].x = 0*l;
    render[2].getTexCoords()[2].x = 0.5*l;
    render[2].getTexCoords()[1].x = 0.25*l;

    render[3].getVertices()[0].x = b1;
    render[3].getVertices()[2].x = b2;
    render[3].getVertices()[1].x = (b1+b2)/2;
    
    render[3].getTexCoords()[0].x = 0.5*l;
    render[3].getTexCoords()[2].x = 1*l;
    render[3].getTexCoords()[1].x = 0.75*l;
}

void Mirror::setTextureMapping(int mappingID)
{
    //sets on the meshes the texture coordinats to the mapping
    //also a fucked up version
    switch (mappingID) {
        case MAPPING::CW:
            
            
            break;
            
        default:
            break;
    }
    
    
}


void Mirror::update(ofTexture & tex)//get shader values and draw the fbo and then to pixels
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
        //if all black
    }
    previewTex = tex;
    ofSetColor(0);
    ofDrawRectangle(drawarea);
    ofSetColor(255);
    ofSetLineWidth(2);
    
    //draw to the artnet fbos bestehend aus den 2x2 segmnenten
    all[0].begin();
    ofClear(0,0,0);
    previewTex.bind();
    if(enabled[0])render[0].draw();
    if(enabled[1])render[1].draw();
    all[0].end();
    
    all[1].begin();
    ofClear(0,0,0);
    if(enabled[2])render[2].draw();
    if(enabled[3])render[3].draw();
    all[1].end();
    ofSetLineWidth(1);
    
    
    previewTex.unbind();

    //here read to universes the fbos and send
    all[0].readToPixels(outPixelsA);
    all[1].readToPixels(outPixelsB);

}

void Mirror::drawPreview(ofTexture &tex)
{
    ofSetLineWidth(3);
    previewTex.bind();
    if(enabled[0])preview[0].draw();
    if(enabled[1])preview[1].draw();
    if(enabled[2])preview[2].draw();
    if(enabled[3])preview[3].draw();
    previewTex.unbind();
    ofSetLineWidth(1);
  
//draw fbos
    all[0].draw(drawarea.getLeft()-50,drawarea.getBottom(),drawarea.getWidth()+100,10);
    all[1].draw(drawarea.getLeft()-50,drawarea.getBottom()+10,drawarea.getWidth()+100,10);
    
}

void Mirror::setEnables(bool left,bool top, bool right, bool bottom)
{
    enabled[0] = left;
    enabled[1] = top;
    enabled[2] = right;
    enabled[3] = bottom;
    //calculate the fbo or pixels
}
