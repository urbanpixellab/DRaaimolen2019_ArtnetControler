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

class ArtnetData
{
public:

    struct Node
    {
        string      ip;
        ofPixels    universes[8];
        ofxArtnetSender artnets[8];
    };
    
    
    
    
    ArtnetData();
    ~ArtnetData();

    void loadNodes();
    void saveNodes();
    
    void send(int &node, int &universum);
    
private:
    vector<Node> _nodes;

};

#endif /* ArtnetData_hpp */
