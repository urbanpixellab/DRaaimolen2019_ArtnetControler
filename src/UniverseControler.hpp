//
//  UniverseControler.hpp
//  DRaaimolen2019_ArtnetControler
//
//  Created by Enrico Becker on 05/09/2019.
//

#ifndef UniverseControler_hpp
#define UniverseControler_hpp

#include <stdio.h>
#include "UniverseMapper.hpp"

class UniverseControler
{
public:
    UniverseControler(ofRectangle drawarea,ofTrueTypeFont *f);
    ~UniverseControler();
    
    void draw();
    
    void editUniverse(int id);
    
    void mousePressed(ofMouseEventArgs &args);

    void setActive(bool value){isActive = value;};
    bool &getActive(){return isActive;};

private:
    ofRectangle                 drawarea;
    ofTrueTypeFont              *mFont;
    vector<UniverseMapper*>     universes;
    bool                        isActive;
    int                         uSelect;//the selected universe for edit
    vector<ofRectangle>         uBtn;
};

#endif /* UniverseControler_hpp */
