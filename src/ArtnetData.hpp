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

class ArtnetData
{
public:
    struct Segment
    {
        int         beginPixel; // start pixel position
        int         endPixel;
        ofPolyline  line;
        int         uniID;      //which universum
        int         nodeID;     //which artnet node
        ofFbo       fbo;        // the fbo of the segment
    };

    struct Spiegel
    {
        Segment seg[4];
    };

    struct Node
    {
        string      ip;
        ofPixels    universes[8];
    };
    
    
    
    
    ArtnetData();
    ~ArtnetData();

    void loadNodes();
    void saveNodes();
    
    void drawPreview(bool * enables);
    
    Node &getNode(int &id){return nodes[id];};
    //Segment &getSegment(int &id){return segments[id];};
    
private:
    vector<Node> nodes;
    vector<Spiegel> spiegels;
};

#endif /* ArtnetData_hpp */
