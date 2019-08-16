//
//  ArtnetData.cpp
//  Sequencer
//
//  Created by Enrico Becker on 01/08/2019.
//

#include "ArtnetData.hpp"

ArtnetData::ArtnetData()
{
    loadNodes();
    int nMAX = 5;
    for(int i = 0;i < 5*8;i++)
    {
        Node n;
        _nodes.push_back(n);
        //every artnet object consists of an ip and an universe
        string ip = "192.168.12." + ofToString(29+floor(i/8.));
        _nodes.back().artnets[i%8].setup(ip,i%8);// everyone has 8 universes
    }

}
ArtnetData::~ArtnetData(){}

void ArtnetData::loadNodes()
{
}

void ArtnetData::send(int &node, int &universum)
{
    //_nodes[node].artnets[universum].sendArtnet(_nodes[node].universes[universum]);
}


void ArtnetData::saveNodes()
{
    
}
