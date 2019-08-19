//
//  Mirror.hpp
//  DraaiLed2019
//
//  Created by Enrico Becker on 15/08/2019.
//
//

#ifndef Mirror_hpp
#define Mirror_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ArtnetData.hpp"//gets data from artnet

class Mirror
{
public:
    enum MAPPING
    {
        CW = 0,
        CCW,
        CENTER_OUT,
        OUT_CENTER,
        TOP_DOWN,
        BOTTOM_UP,
        FUCKED_UP
    };
    
    Mirror(int id, ArtnetData * artnet,ofRectangle area);
    ~Mirror();
    
    void setMappingMode(){};
    void setEnables(bool left,bool top, bool right, bool bottom);//also mapping
    void update();//draw the stuff to the arrays
    void drawPreview(ofTexture &tex);
    void setTextureMapping(int mappingID);
    
    
private:
    
    int     mID;//the mirror id
    ofRectangle drawarea;//gets later shifted, in 1led = 1pixel size 
    // can i calculate by the mirror id
    int     myNode; //which artnet node to write
    int     myUniverses[2];// can i calculate by the mirror id

    bool    enabled[4]={true}; //for every segment the enable, first all off
    //think about the segments, which universes
    
    ofPixels    test;
    ofImage     testImg;
    ofMesh      preview[4];//left top right bottom
    ofMesh      render[4];
    //left top right bottom //every render heas 3 coordinates, and the shifted texture coordinates based on the mapping and inverse
    
    
    ArtnetData  *artnet;//direct link to artnet
    int     mappingMode;//how are the segments presented; cw ccw in<->out bottum up top down
    ofFbo       all[2];//all two strips
    
    //audio amplifier
};

#endif /* Mirror_hpp */
