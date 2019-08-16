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
        ofxArtnetSender artnets[8];
    };
    
    
    
    
    ArtnetData();
    ~ArtnetData();

    void loadNodes();
    void saveNodes();
    
    void send(int &node, int &universum);
    void drawPreview(bool * enables);
    
    Node &getNode(int &id){return nodes[id];};
    //Segment &getSegment(int &id){return segments[id];};
    
private:
    vector<Node> nodes;
    vector<Spiegel> spiegels;
    vector<Node> _nodes;

};

#endif /* ArtnetData_hpp */
