//
//  DataControler.cpp
//  Sequencer
//
//  Created by Enrico Becker on 01/08/2019.
//

#include "DataControler.hpp"

DataControler::DataControler()
{
//    loadData();
    //create 16 pattern storage, later fill it by xml
}

DataControler::~DataControler()
{
//    saveData();
}

void DataControler::loadData()
{
//    pData.push_back(PatternData());
}

void DataControler::saveData()
{
    /*
    ofxXmlSettings settings;
    settings.addTag("patterns");
    settings.pushTag("patterns");
    for (int i = 0; i < pData.size(); i++)
    {
        settings.addTag("id");
        settings.pushTag("id",i);
        settings.addValue("ID", i);
        settings.addValue("adsrSelect", pData[i].adsrSelect);
        settings.addValue("adsrInv", pData[i].adsrInv);
        settings.addValue("adsrRev", pData[i].adsrRev);
        settings.addValue("patSelect", pData[i].patternSelect);
        settings.addValue("patDirection", pData[i].patternDirection);
        settings.addTag("seq");
        settings.pushTag("seq");
        for (int s = 0; s < pData[i].sequenzer.size(); s++)
        {
            settings.addValue(ofToString(s), pData[i].sequenzer[s]);
        }
        settings.popTag();
        settings.popTag();
    }
    settings.popTag();
    settings.saveFile("data.xml");
     */
}
