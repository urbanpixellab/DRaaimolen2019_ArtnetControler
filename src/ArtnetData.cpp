//
//  ArtnetData.cpp
//  Sequencer
//
//  Created by Enrico Becker on 01/08/2019.
//

#include "ArtnetData.hpp"

ArtnetData::ArtnetData()
{
    //send test
    test.allocate(150, 1, 3);
    for (int i = 0; i < 150; i++)
    {
        ofColor c = ofColor(ofRandom(255),ofRandom(255),ofRandom(255));
        test.setColor(i, 0, c);//ramp
    }

    int nMAX = 5;
    for(int i = 0;i < nMAX;i++)
    {
        string ip = "192.168.12." + ofToString(29+floor(i));
        for (int u = 0; u < 8; u++)
        {
            ofxArtnetSender *s = new ofxArtnetSender();
            s->setup(ip,u);
            artnets.push_back(s);
        }
    }
    loadNodes();
}
ArtnetData::~ArtnetData()
{
    for(int u = 0; u < artnets.size();u++)
    {
        delete artnets[u];
    }
}

void ArtnetData::loadNodes()
{
}


void ArtnetData::send(int uID,ofPixels & pix)
{
    artnets[uID]->sendArtnet(pix);
}

void ArtnetData::sendMirror(int &uID,ofPixels & pixA,ofPixels & pixB)
{
    artnets[uID]->sendArtnet(pixA);
    artnets[uID+1]->sendArtnet(pixB);
}

void ArtnetData::sendAll(ofPixels &pix)
{
    for (int i = 0; i < artnets.size(); i++)
    {
        artnets[i]->sendArtnet(pix);
    }
}



void ArtnetData::saveNodes()
{
}
