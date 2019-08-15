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
}
ArtnetData::~ArtnetData(){}

void ArtnetData::loadNodes()
{
    //temporary create instead of load
    nodes.clear();
    int stripLenghth = 150;
    for (int i = 0; i < 1; i++)
    {
        Node n;
        n.ip = "127.0.0.1";
        for (int u = 0; u < 8;u++)
        {
            n.universes[u].allocate(stripLenghth, 1, OF_PIXELS_RGB);
            //fill black
            while( i < stripLenghth*3)
            {
                n.universes[u][i] = 0;
                i++;
            }
        }
        nodes.push_back(n);
    }
    
    spiegels.clear();
    for (int i = 0; i < 20; i++)
    {
        int spacer = 5;
        int w = 30;
        int h = 90;
        int x = i * (w + spacer);
        int y = 0;
        ofRectangle area(x,y,w,h);
        Spiegel s;
        s.seg[0].line.addVertex(x, y+h); //links cw
        s.seg[0].line.addVertex(x, y);

        s.seg[1].line.addVertex(x, y); //oben
        s.seg[1].line.addVertex(x+w, y);

        s.seg[2].line.addVertex(x+w, y);//rechts
        s.seg[2].line.addVertex(x+w, y+h);

        s.seg[3].line.addVertex(x+w, y+h);//unten
        s.seg[3].line.addVertex(x, y+h);

        spiegels.push_back(s);
    }
}

void ArtnetData::drawPreview(bool * enables)
{
    for(int i = 0;i < spiegels.size();i++)
    {
        for (int s = 0;s < 4; s++)
        {
            if(enables[i*4 + s] == true) spiegels[i].seg[s].line.draw();
        }
    }
}

void ArtnetData::saveNodes()
{
    
}
