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
    
    
    
    void send(int &node, int &universum);
    void send(int &node, int &universum, ofPixels &pix);
    
    Node *getNode(int &id){return _nodes[id];};
    
    void sendAll(ofPixels &pix);
    
private:
    vector<Node*>               _nodes;
    vector<ofxArtnetSender*>     artnets;
    ofPixels                    test;
};

#endif /* ArtnetData_hpp */
