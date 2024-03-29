#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    /*  todo
        texture mapping
        color flash, black flash, freeze, invert,trace, sparkles
        color shift by index multiplied/add by curve and freq
        color freq shift by curve, id and freq
        midi controler
    */
    isFlickering = false;
    autoMode = false;
    isNewLIveSelect = false;
    ofSetBackgroundColor(31,27,33);
    steplength = 0.125;
    copieID = -1;
    ofSetFrameRate(30);
    timer = 0;
    menueFont.load("verdana.ttf", 8);
    editSelect = 0;
    liveSelect = 0;
    for(int i = 0;i < 16 ;i++)
    {
        patEditors.push_back(new PatternEditor(ofRectangle(0,0,ofGetWidth(),ofGetHeight()),i,&menueFont));
        ofAddListener(patEditors.back()->isTrigger, this, &ofApp::isMirrorTrigger);
    }
    PREVIEW = new PatternEditor();
    PREVIEW = patEditors[editSelect];
    LIVE = new PatternEditor();
    LIVE = patEditors[liveSelect];
    
    //preview buttons
    previewBTNs.clear();
    liveBTNs.clear();
    float cx = ofGetWidth()/2;
    float cy = ofGetHeight()/2;
    int w = ofGetWidth() / 32;
    float radius = (ofGetHeight()/2) * 0.8;
    for(int i = 0;i < patEditors.size();i++)
    {
        float x = (ofGetWidth() - w*5) + (i%4*w*1.1);
        float y = w + floor(i/4)*w*1.1;
        previewBTNs.push_back(ofRectangle(x-w/2,y-w/2,w,w));
        liveBTNs.push_back(ofRectangle(x-w/2,250+y-w/2,w,w));
    }

    artnet = new ArtnetData();
    
    w = ofGetWidth() / 40;
    int h = 90;//we have max 90 leds in height
    for (int i = 0; i < 20; i++)
    {
        float x = cx + radius*sin((i/20.)*TWO_PI);
        float y = cy + radius*cos((i/20.)*TWO_PI);
        int startUniversum = (i%4)+(i%4);
        cout << startUniversum << endl;
        mirrors.push_back(Mirror(i, artnet,ofRectangle(x-w/2,y-h/2,w,h),startUniversum,&gfx));
    }
    PREVIEW->setActive(true);

    masterBrightness = new RotaryEncoder(ofRectangle(10,10,100,100), 20, &menueFont, "BRIGHTNESS", 0, 1, 10, false);
    masterBrightness->setActive(true);
    masterClock = 0;
    loadPixelMapping();
    w = 100;
    h = 100;
    ofSetColor(128);
    buttons[0] = ofRectangle((ofGetWidth()/2)-w*1.1,(ofGetHeight()/2)-h/2,w,h);
    buttons[1] = ofRectangle((ofGetWidth()/2)+w*0.1,(ofGetHeight()/2)-h/2,w,h);
    ofSetColor(0);
//    menueFont.drawString("FLASH", buttons[0].getX(),buttons[0].getBottom());
//    menueFont.drawString("FREEZE", buttons[1].getX(),buttons[1].getBottom());
    TapTempo = new Tap(ofRectangle(210,10,100,100),&menueFont);
    ofAddListener(TapTempo->newTimer, this, &ofApp::newTapTempo);
    loadPatternEditorSettings();
    autoBtn = ofRectangle(800,20,100,100);
}

void ofApp::savePatternEditorSettings()
{
    ofxXmlSettings  set;
    for (int i = 0; i < patEditors.size(); i++)
    {
        set.addTag("Editor");
        set.pushTag("Editor",i);
        set.addValue("ID", i);
        //mirror enables
        set.addValue("mirrorSegmentID", patEditors[i]->getMirrorPatternGen().getPatternID());
        set.addValue("mirrorSubSegmentID", patEditors[i]->getMirrorSubPatternGen().getPatternID());
        set.addValue("mirrorCurveID", patEditors[i]->getMirrorCurve().getCurveID());// the mirror curve
        //color
        set.addValue("colorA", patEditors[i]->getColorSwatch().getColorAID());
        set.addValue("colorB", patEditors[i]->getColorSwatch().getColorBID());
        set.addValue("ColorFreq", patEditors[i]->getColorFreqEncoder().getValueNormalized());
        set.addValue("ColorShift", patEditors[i]->getColorShiftEncoder().getValueNormalized());
        set.addValue("colorCurveID", patEditors[i]->getColorCurve().getCurveID());
        set.addTag("SeqA");
        set.pushTag("SeqA",0);
        for(int s = 0;s < patEditors[i]->getSequenzer(0).getSteps().size();s++)
        {
            set.addValue("step", patEditors[i]->getSequenzer(0).getSteps()[s].pressed);
        }
        set.popTag();
        set.addTag("SeqB");
        set.pushTag("SeqB",0);
        for(int s = 0;s < patEditors[i]->getSequenzer(0).getSteps().size();s++)
        {
            set.addValue("step", patEditors[i]->getSequenzer(1).getSteps()[s].pressed);
        }
        set.popTag();

        set.popTag();
    }
    set.save("patterns.xml");
}



void ofApp::loadPatternEditorSettings()
{
    cout << "load patterneditor" << endl;
    ofxXmlSettings  set;
    steplength = 0.125;
    set.load("patterns.xml");
    int num = set.getNumTags("Editor");
    
    for (int i = 0; i < num; i++)
    {
        set.pushTag("Editor",i);
        set.getValue("Editor",i);
        int id = set.getValue("ID", i);
        //cout << "id " << id << endl;
        int colorA = set.getValue("colorA", 0);
        int colorB = set.getValue("colorB", 0);
        patEditors[id]->getColorSwatch().setColorA(colorA);
        patEditors[id]->getColorSwatch().setColorB(colorB);
//        int mSubPatID = set.getValue("mirrorSubSegmentID", 0);
//        patEditors[id]->getMirrorSubPatternGen().getEncoder().setValue(mSubPatID);
        set.popTag();
    }
    /*
     ofxXmlSettings  set;
     for (int i = 0; i < patEditors.size(); i++)
     {
     set.addTag("Editor");
     set.pushTag("Editor",i);
     set.addValue("ID", i);
     //mirror enables
     set.addValue("mirrorSegmentID", patEditors[i]->getMirrorPatternGen().getPatternID());
     set.addValue("mirrorSubSegmentID", patEditors[i]->getMirrorSubPatternGen().getPatternID());
     set.addValue("mirrorCurveID", patEditors[i]->getMirrorCurve().getCurveID());// the mirror curve
     //color
     set.addValue("colorA", patEditors[i]->getColorSwatch().getColorAID());
     set.addValue("colorB", patEditors[i]->getColorSwatch().getColorBID());
     set.addValue("ColorFreq", patEditors[i]->getColorFreqEncoder().getValueNormalized());
     set.addValue("ColorShift", patEditors[i]->getColorShiftEncoder().getValueNormalized());
     set.addValue("colorCurveID", patEditors[i]->getColorCurve().getCurveID());
     set.addTag("SeqA");
     set.pushTag("SeqA",0);
     for(int s = 0;s < patEditors[i]->getSequenzer(0).getSteps().size();s++)
     {
     set.addValue("step", patEditors[i]->getSequenzer(0).getSteps()[s].pressed);
     }
     set.popTag();
     set.addTag("SeqB");
     set.pushTag("SeqB",0);
     for(int s = 0;s < patEditors[i]->getSequenzer(0).getSteps().size();s++)
     {
     set.addValue("step", patEditors[i]->getSequenzer(1).getSteps()[s].pressed);
     }
     set.popTag();
     
     set.popTag();
     }
     set.save("patterns.xml");

    */
}

void ofApp::newTapTempo(float & newT)
{
    masterClock = 0;
    steplength = newT;
    timer = ofGetElapsedTimef();
    //aslo add a reset 
}


//--------------------------------------------------------------
void ofApp::update()
{
    float now = ofGetElapsedTimef();
    if(now >= timer + steplength)
    {
        //first update the gfx
        masterClock++;
        if (masterClock >= 64) masterClock = 0;
        //set all colors, should been done somewhere else
        timer = now + steplength;
        //update all
        for (int i = 0; i < patEditors.size(); i++)
        {
            patEditors[i]->nextStep(masterClock);
        }
    }
    else
    {
        //update all
        for (int i = 0; i < patEditors.size(); i++)
        {
            patEditors[i]->update();
        }
    }
    //int liveSelect = li;
    // now write to artnet
    for(int i = 0;i < mirrors.size();i++)
    {
        if (isFlickering)
        {
            mirrors[i].flickeringLights(LIVE->getColorA1());
        }
        else if(isFlash == false)
        {
            float shiftPre = PREVIEW->getColorShift();
            gfx.drawToPreviewFbo(mirrors[i].getPreFbo(),PREVIEW->getCurve(),PREVIEW->getColorDelta(),PREVIEW->getValueA(),masterBrightness->getValue(),PREVIEW->getColorFreqNorm(),shiftPre,PREVIEW->getColorA1(), PREVIEW->getColorA2());
            
            float f = LIVE->getColorFreqNorm()+fmod(i*LIVE->getColorFreqOff()*0.2,1.0);
            float shiftLive = LIVE->getColorShift() + (0.5 + sin(i*TWO_PI)*0.5);
            gfx.drawToFbo(mirrors[i].getLiveFbo(),LIVE->getCurve(),LIVE->getColorDelta(),LIVE->getValueA(),masterBrightness->getValue(),f,shiftLive,LIVE->getColorA1(), LIVE->getColorA2());
        }
        else
        {
            float shiftPre = PREVIEW->getColorShift();
            gfx.drawToPreviewFbo(mirrors[i].getPreFbo(),PREVIEW->getWhite(),PREVIEW->getColorDelta(),PREVIEW->getValueA(),masterBrightness->getValue(),PREVIEW->getColorFreqNorm(),shiftPre,PREVIEW->getColorA1(), PREVIEW->getColorA2());
            
            float shiftLive = LIVE->getColorShift();
            gfx.drawToFbo(mirrors[i].getLiveFbo(),LIVE->getWhite(),LIVE->getColorDelta(),LIVE->getValueA(),masterBrightness->getValue(),LIVE->getColorFreqNorm(),shiftLive,LIVE->getColorA1(), LIVE->getColorA2());

        }
        mirrors[i].updateLive();
        if(!isFreeze)
        {
            artnet->send((i*2)+0,mirrors[i].getPixelsA());
            artnet->send((i*2)+1,mirrors[i].getPixelsB());
        }
    }
    if(autoMode)
    {
        if (masterClock == 0 && isNewLIveSelect == false)
        {
            setLiveID(ofRandom(patEditors.size()));
            isNewLIveSelect = true;
        }
    }
    
//    artnet->sendAll(mirrors[0].getAllWhite());
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    PREVIEW->drawGUI();
    int id = 0;
    for(int i = 0;i < previewBTNs.size();i++)
    {
        ofSetColor(255,128,0);
        ofDrawRectangle(previewBTNs[i]);
        ofSetColor(255);
        menueFont.drawString(ofToString(i+1), previewBTNs[i].getCenter().x,previewBTNs[i].getCenter().y);
        //the live buttons
        ofSetColor(0,128,255);
        ofDrawRectangle(liveBTNs[i]);
        ofSetColor(255);
        menueFont.drawString(ofToString(i+1), liveBTNs[i].getCenter().x,liveBTNs[i].getCenter().y);
    }

    // now tghe selected button
    ofNoFill();
    ofSetLineWidth(3);
    ofSetColor(255);
    ofDrawRectangle(previewBTNs[editSelect]);
    ofDrawRectangle(liveBTNs[liveSelect]);
    ofFill();
    ofSetLineWidth(1);
    
    ofSetColor(255);
    ofDrawBitmapString("fps " + ofToString(ofGetFrameRate()),0,20);

    for(int i = 0;i < mirrors.size();i++)
    {
        mirrors[i].drawPreview();
        mirrors[i].drawLive();
    }
    
    masterBrightness->draw();
    
    if(!isFlash)
    {
        ofSetColor(255);
        ofDrawRectangle(buttons[0]);
    }
    else{
        ofSetColor(255,0,0);
        ofDrawRectangle(buttons[0]);
    }
    
    if(!isFreeze)
    {
        ofSetColor(255);
        ofDrawRectangle(buttons[1]);
    }
    else
    {
        ofSetColor(255,0,0);
        ofDrawRectangle(buttons[1]);
    }
    if(!autoMode)
    {
        ofSetColor(255);
        ofDrawRectangle(autoBtn);
    }
    else
    {
        ofSetColor(255,0,0);
        ofDrawRectangle(autoBtn);
    }
    
    ofSetColor(0);
    menueFont.drawString("FLASH", buttons[0].getLeft(),buttons[0].getBottom());
    menueFont.drawString("FREEZE", buttons[1].getLeft(),buttons[1].getBottom());
    menueFont.drawString("BEER", autoBtn.getLeft(),autoBtn.getBottom());
    TapTempo->draw();

}

void ofApp::setEditorID(int index)
{
    // first disable the old selection
    PREVIEW->setActive(false);
    editSelect = index;
    PREVIEW = patEditors[index];
    PREVIEW->setActive(true);
}

void ofApp::setLiveID(int index)
{
    liveSelect = index;
    LIVE = patEditors[liveSelect];
}

void ofApp::isMirrorTrigger(int &triggerIndex)
{
    if(triggerIndex != editSelect && triggerIndex != liveSelect) return;
    if(triggerIndex == editSelect) // the pattern for segments , 1 = color
    {
        for(int i = 0;i < mirrors.size();i++)
        {
            if(PREVIEW->getMirrorPattern()[i] == true)
            {
                bool left = PREVIEW->getMirrorSubPattern(i)[0];
                bool top = PREVIEW->getMirrorSubPattern(i)[1];
                bool right = PREVIEW->getMirrorSubPattern(i)[2];
                bool bottom = PREVIEW->getMirrorSubPattern(i)[3];
                mirrors[i].setEnablesPre(left,top,right,bottom);
                //cout << "mirror" << i << " : " << left << " " << top << " " << right << " " << bottom << endl;
            }
            else
            {
                mirrors[i].setEnablesPre(false,false,false,false);
            }
        }
    }
    if(triggerIndex == liveSelect)
    {
        for(int i = 0;i < mirrors.size();i++)
        {
            if(LIVE->getMirrorPattern()[i] == true)
            {
                bool left = LIVE->getMirrorSubPattern(i)[0];
                bool top = LIVE->getMirrorSubPattern(i)[1];
                bool right = LIVE->getMirrorSubPattern(i)[2];
                bool bottom = LIVE->getMirrorSubPattern(i)[3];
                mirrors[i].setEnablesLive(left,top,right,bottom);
                //cout << "mirror" << i << " : " << left << " " << top << " " << right << " " << bottom << endl;
            }
            else
            {
                mirrors[i].setEnablesLive(false,false,false,false);
            }
        }
    }
}

void ofApp::loadPixelMapping()
{
    ofxXmlSettings settings;
    settings.load("PixelMap.xml");
    cout << "num u " << settings.getNumTags("universe") << endl;
    
    for (int i = 0; i < 20; i++)
    {
        settings.pushTag("universe",i);
        int id = settings.getValue("id", 0);
        int l1 = settings.getValue("l1", 0);
        int l2 = settings.getValue("l2", 0);
        int t1 = settings.getValue("t1", 0);
        int t2 = settings.getValue("t2", 0);
        int r1 = settings.getValue("r1", 0);
        int r2 = settings.getValue("r2", 0);
        int b1 = settings.getValue("b1", 0);
        int b2 = settings.getValue("b2", 0);
        //cout << "id " << id << " l1 " << l1 << " l2 " << l2 << endl;
        // depending on the id, set the mirror points
        cout << l1 << ":" << l2 << ":" << t1 << ":" << t2 << ":" << r1 << ":" << r2 << ":" << b1 << ":" << b2 << endl;
        mirrors[id].setUniverses(l1,l2,t1,t2,b1,b2,r1,r2);
        settings.popTag();
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'r')//reload settings
    {
        loadPixelMapping();
        artnet->loadNodes();
        gfx.reloadShader();
        for (int i = 0; i < patEditors.size(); i++)
        {
            patEditors[i]->getColorSwatch().loadColors();
        }
    }
    if(key == ' ')//reload settings
    {
        mirrors[4].deb();
        cout << "debug" << endl;
    }
    if(key == 'f')//reload settings
    {
        isFlickering = !isFlickering;
        cout << "flickering" << endl;
    }
}

void ofApp::exit()
{
    savePatternEditorSettings();
    for(int i = 0;i < patEditors.size();i++)
    {
        delete patEditors[i];
    }
    delete artnet;
    delete masterBrightness;
    delete TapTempo;
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if(copieID < 0)return;
    //else we copie a preset
    
    //and set on release
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    copieID = -1;
    for(int i = 0;i < previewBTNs.size();i++)
    {
        if(previewBTNs[i].inside(x, y))
        {
            setEditorID(i);
            copieID = i;
            
        }
    }
    for(int i = 0;i < liveBTNs.size();i++)
    {
        if(liveBTNs[i].inside(x, y))
        {
            setLiveID(i);
        }
    }
    if(buttons[0].inside(x,y))
    {
        //flash
        isFlash = true;
    }
    if(buttons[1].inside(x,y))
    {
        //flash
        isFreeze = true;
    }
    if(autoBtn.inside(x,y))
    {
        //flash
        autoMode = !autoMode;
        autoSteps = ofRandom(10);
        if (autoMode == false) isNewLIveSelect = false;
    }

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    isFlash = false;
    isFreeze = false;

    if(copieID < 0)return;
    bool isCopie = false;
    for(int i = 0;i < previewBTNs.size();i++)
    {
        if(previewBTNs[i].inside(x,y) == true && i != copieID)
        {
            //we have a hit the copie if i am not myself
            patEditors[i] = patEditors[copieID];
            cout << "copied id " << copieID <<  " to id " << i << endl;
        }
    }

    
    //check if we are in a new valid field and the copie if valid
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    cout << w << " " << h << endl;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
