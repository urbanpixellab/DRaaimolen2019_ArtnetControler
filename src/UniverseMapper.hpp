//
//  UniverseMapper.hpp
//  DraaiLed2019
//
//  Created by Enrico Becker on 22/08/2019.
//
//

#ifndef UniverseMapper_hpp
#define UniverseMapper_hpp

#include <stdio.h>
#include "ofMain.h"

class UniverseMapper
{
public:
    
    struct Segment
    {
        int pixelIDs[3];//begin center end
        ofVec2f texCoords[3];//begin center end
        int lengthPixels;
        bool isInverse;
        vector<int> pixels;
    };
    
    UniverseMapper(ofRectangle area,int length,ofTrueTypeFont *f);
    ~UniverseMapper();
    
    void draw();
    void updateSegment(int id,int beginPixel,int endPIxel);
    void mousePressed(ofMouseEventArgs & args);
    void mouseDragged(ofMouseEventArgs & args);
    void mouseReleased(ofMouseEventArgs & args);
    

private:
    void updateFbo();
    
    ofRectangle drawarea;
    vector<ofRectangle> areas;
    ofFbo       drawFbo;
    ofFbo       buttonOverlay;
    ofTrueTypeFont *mFont;
    
    Segment     mirSeg[2];//hold two segments for thi universe
    int selectedPointID;//-1 no point, 0=3 in1 out1 in2 out2
    int         ioPixelSelect;//is it a ioPixel
    
};

#endif /* UniverseMapper_hpp */
