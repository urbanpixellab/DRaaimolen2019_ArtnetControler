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

    loadNodes();
    int nMAX = 5;
    for(int i = 0;i < nMAX;i++)
    {
        Node *n = new Node();
        //every artnet object consists of an ip and an universe
        string ip = "192.168.12." + ofToString(29+floor(i));
        for (int u = 0; u < 8; u++)
        {
            n->artnets[u] = new ofxArtnetSender();
            n->artnets[u]->setup(ip);// everyone has 8 universes
            //n->artnets[u]->sendArtnet(test);
            ofxArtnetSender *s = new ofxArtnetSender();
            s->setup(ip,u);
            artnets.push_back(s);
        }
        _nodes.push_back(n);
    }
}
ArtnetData::~ArtnetData()
{
    for (int i = 0; i < _nodes.size(); i++)
    {
        for(int u = 0; u < 8;u++)
        {
            delete _nodes[i]->artnets[u];
        }
        delete _nodes[i];
    }
    for(int u = 0; u < artnets.size();u++)
    {
        delete artnets[u];
    }
}

void ArtnetData::loadNodes()
{
}

void ArtnetData::send(int &node, int &universum)
{
    //_nodes[node].artnets[universum].sendArtnet(_nodes[node].universes[universum]);
}

void ArtnetData::send(int &node, int &universum,ofPixels & pix)
{
    cout << "send " << node  << " universum " << universum << endl;

    _nodes[node]->artnets[universum]->sendArtnet(pix);
}

void ArtnetData::sendTest()
{
    for (int i = 0; i < artnets.size(); i++)
    {
        artnets[i]->sendArtnet(test);
    }
}

void ArtnetData::sendTest2(ofPixels &pix)
{
    for (int i = 0; i < artnets.size(); i++)
    {
//        artnets[i]->sendArtnet(pix);
    }
}


void ArtnetData::saveNodes()
{
    
}
