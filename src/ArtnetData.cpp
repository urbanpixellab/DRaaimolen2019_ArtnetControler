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
        Node *n = new Node();
        //every artnet object consists of an ip and an universe
        string ip = "192.168.12." + ofToString(29+floor(i));
        for (int u = 0; u < 8; u++)
        {
            n->artnets[u] = new ofxArtnetSender();
            n->artnets[u]->setup(ip,u);// everyone has 8 universes
            //n->artnets[u]->sendArtnet(test);
            ofxArtnetSender *s = new ofxArtnetSender();
            s->setup(ip,u);
            artnets.push_back(s);
        }
        _nodes.push_back(n);
    }
    loadNodes();
}
ArtnetData::~ArtnetData()
{
    saveNodes();
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
    ofxXmlSettings set;
    set.load("artnet.xml");
    
    for (int i = 0; i < set.getNumTags("Node"); i++)
    {
        set.pushTag("Node",i);
        string ip = set.getValue("IP", "127.0.0.1");
        for (int u = 0; u < 8; u++)
        {
            _nodes[i]->artnets[u]->setup(ip);
            cout << "Node " << i << " with ip " << ip << " " << u << " loaded" << endl;
        }
        set.popTag();
        
    }
}

void ArtnetData::send(int &node, int &universum,ofPixels & pix)
{
    cout << "send " << node  << " universum " << universum << endl;
    _nodes[node]->artnets[universum]->sendArtnet(pix);
}

void ArtnetData::send(int &uID,ofPixels & pix)
{
//    artnets[uID]->sendArtnet(pix);
}

void ArtnetData::sendAll(ofPixels &pix)
{
    for (int i = 0; i < artnets.size(); i++)
    {
//        artnets[i]->sendArtnet(pix);
    }
}



void ArtnetData::saveNodes()
{
    ofxXmlSettings set;
    for (int i = 0; i < _nodes.size(); i++)
    {
        set.addTag("Node");
        set.pushTag("Node",i);
        string ip = "192.168.12." + ofToString(29+floor(i));
        set.addValue("IP", ip);
        set.popTag();

    }
    
    set.save("artnet.xml");
    
}
