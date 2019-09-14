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
#include "GraphicGenerator.hpp"

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
    
    Mirror(int id, ArtnetData * artnet,ofRectangle area,int startUniversum,GraphicGenerator *gfx);
    ~Mirror();
    
    void createUniverses();
    void setMappingMode(){};
    void setEnablesPre(bool left,bool top, bool right, bool bottom);
    void setEnablesLive(bool left,bool top, bool right, bool bottom);//also mapping
    void allOff();
    void setMappingMode(int mode);
    void updateLive();
    void drawPreview();
    void drawLive();
    void deb();
    
    
    void setTextureMapping(int mappingID);
    ofPixels &getPixelsA(){return outPixelsA;};
    ofPixels &getPixelsB(){return outPixelsB;};
    ofPixels &getAllBlack(){return allBlack;};
    ofPixels &getAllWhite(){return whitePix;};
    ofPixels &getAllRed(){return redPix;};
    int &getUniverseID(int id){return myUniverses[id];};
    
    ofFbo &getFbo(int id){return all[id];};
    // now the setup by xml
    void setUniverses(int &l1, int &l2, int &t1, int &t2,int &r1, int &r2, int &b1, int &b2);
    
    ofFbo &getPreFbo(){return preFbo;};
    ofFbo &getLiveFbo(){return liveFbo;};
    void flickeringLights(ofColor &colorA);
    
private:
    
    int     mID;//the mirror id
    ofRectangle drawarea;//gets later shifted, in 1led = 1pixel size 
    // can i calculate by the mirror id
    int     myNode; //which artnet node to write
    int     myUniverses[2];// can i calculate by the mirror id

    bool    enPre[4]={true};
    bool    enLive[4]={true};
    
    ofPixels    outPixelsA;
    ofPixels    outPixelsB;
    ofImage     outImgA;
    ofImage     outImgB;
    ofMesh      preview[4];//left top right bottom
    ofMesh      render[4];//for the linear fbo
    //left top right bottom
    //every render heas 3 coordinates, and the shifted texture coordinates based on the mapping and inverse
    
    
    ArtnetData  *artnet;//direct link to artnet
    int     mappingMode;//how are the segments presented; cw ccw in<->out bottum up top down
    ofFbo       all[2];//all two strips
    ofPixels    allBlack;//for faster sending off black
    //audio amplifier
    GraphicGenerator    *gfx;
    ofPixels            whitePix;
    ofPixels            redPix;
    ofTexture           previewTex;
    ofFbo               preFbo;
    ofFbo               liveFbo;
};

#endif /* Mirror_hpp */
