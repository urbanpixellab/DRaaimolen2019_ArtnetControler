//
//  DataControler.hpp
//  Sequencer
//
//  Created by Enrico Becker on 01/08/2019.
//

#ifndef DataControler_hpp
#define DataControler_hpp

#include <stdio.h>
#include "ofMain.h"
#include "PatternData.hpp"
#include "ofxXmlSettings.h"

class DataControler
{
public:
    DataControler();
    ~DataControler();

    void loadData();//loads the data from xml
    void saveData();//save the data to xml
    
    //load from data
};

#endif /* DataControler_hpp */
