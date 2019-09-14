//
//  ArtnetData.hpp
//  Sequencer
//
//  Created by Enrico Becker on 01/08/2019.
//  holds the artnet data

#ifndef ArtnetData_hpp
#define ArtnetData_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxArtnet.h"
#include "ofxXmlSettings.h"

class ArtnetData
{
public:

    struct Node
    {
        string      ip;
        ofPixels    universes[8];
        ofxArtnetSender *artnets[8];
    };
    
    
    
    
    ArtnetData();
    ~ArtnetData();

    void loadNodes();
    void saveNodes();
    
    
    
    void send(int uID, ofPixels &pix);
    void sendMirror(int &uID, ofPixels &pixA,ofPixels &pixB);
    
    void sendAll(ofPixels &pix);
    
    
private:
    vector<ofxArtnetSender*>     artnets;
    ofPixels                    test;
};

#endif /* ArtnetData_hpp */
