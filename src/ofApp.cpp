#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(31,27,33);
    steplength = 0.125;
    cout << "init steptime " << steplength << endl;
    
    ofSetFrameRate(30);
    timer = 0;
    menueFont.load("verdana.ttf", 8);
    editSelect = 0;
    liveSelect = 0;
    preTex.allocate(100, 1);
    
    for(int i = 0;i < 16 ;i++)
    {
        patEditors.push_back(new PatternEditor(ofRectangle(0,0,ofGetWidth(),ofGetHeight()),&menueFont));
        ofAddListener(patEditors.back()->isTrigger, this, &ofApp::isTrigger);
    }
    //LIVE = new PatternEditor();
    
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
    //LIVE = patEditors[liveSelect];
    
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
    patEditors[editSelect]->setActive(true);
    masterBrightness = new RotaryEncoder(ofRectangle(10,10,100,100), 20, &menueFont, "BRIGHTNESS", 0, 1, 10, false);
    masterBrightness->setActive(true);
    masterClock = 0;
    loadPatternEditorSettings();
    loadPixelMapping();
    w = 100;
    h = 100;
    ofSetColor(128);
    buttons[0] = ofRectangle((ofGetWidth()/2)-w*1.1,(ofGetHeight()/2)-h/2,w,h);
    buttons[1] = ofRectangle((ofGetWidth()/2)+w*0.1,(ofGetHeight()/2)-h/2,w,h);
    ofSetColor(0);
    menueFont.drawString("FLASH", buttons[0].getX(),buttons[0].getBottom());
    menueFont.drawString("INVERT", buttons[1].getX(),buttons[1].getBottom());
    
    
    cout << "load steptime " << steplength << endl;
}

void ofApp::savePatternEditorSettings()
{
    ofxXmlSettings  set;
    for (int i = 0; i < patEditors.size(); i++)
    {
        set.addTag("Editor");
        set.pushTag("Editor",i);
        set.addValue("ID", i);
        set.addValue("mirColorFreq", patEditors[i]->getColorFreq());
        //patEditors[i]->get
//        float mirrorSelect = ;

        //now add the interesting items with get and set
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
        cout << "id " << id << endl;
        //steplength = 2;
        float freQ = set.getValue("mirColorFreq", 0.);
        patEditors[i]->getColorFreq() = freQ;//set.getValue("mirColorFreq", 0.);
        cout << patEditors[i]->getColorFreq() << endl;
        
        patEditors[i]->update();
        //now add the interesting items with get and set
        set.popTag();
    }
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
        for(int i = 0;i < mirrors.size();i++)
        {
            gfx.setColor(patEditors[editSelect]->getColorA1(), patEditors[editSelect]->getColorA2());
        }
        timer = now + steplength;
        //update all
        for (int i = 0; i < patEditors.size(); i++)
        {
            patEditors[i]->nextStep();
        }
        //do all

        stepcount++;
        if(stepcount >= 16)
        {
            stepcount = 0;
        }

        //if(editSelect != liveSelect) LIVE->nextStep(); // onl;y if they are not the same update them
    }
    else
    {
        for (int i = 0; i < patEditors.size(); i++)
        {
        //    patEditors[editSelect]->nextStep();
        }
        patEditors[editSelect]->update();
        //LIVE->update();
    }
    ofFbo liveTex;
    int liveSelect = editSelect;
    
    float d = patEditors[editSelect]->getColorDelta();// the sequenzer delta
    //order ofFbo &screen,ofTexture &tex,float &delta,float &bright,ofColor &a,ofColor &b,float &freq,float &shift
    gfx.drawToFbo(preTex,patEditors[editSelect]->getCurve(),d,patEditors[editSelect]->getValueA(),masterBrightness->getValue(),patEditors[editSelect]->getColorFreq(),patEditors[editSelect]->getColorShift());
    
    // now write to artnet
    
    for(int i = 0;i < mirrors.size();i++)
    {
        //send to artnet
//        mirrors[i].update(preview.getTexture());
        mirrors[i].update(preTex.getTexture());
        //here comes he sequencer in
        
//        artnet->sendTest2(mirrors[i].getPixelsA());
        
        //artnet->send(n,mirrors[i].getUniverseIDA() ,mirrors[i].getPixelsA());
        //artnet->send(n,mirrors[i].getUniverseIDB() ,mirrors[i].getPixelsB());
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
//    ofSetColor(0);
//    ofDrawRectangle(0,0, ofGetWidth(), 150);
    ofSetColor(255);
    patEditors[editSelect]->drawGUI();
    int id = 0;
    for(int i = 0;i < previewBTNs.size();i++)
    {
        ofSetColor(255,128,0);
        ofDrawRectangle(previewBTNs[i]);
        ofSetColor(255);
        menueFont.drawString(ofToString(i+1), previewBTNs[i].getCenter().x,previewBTNs[i].getCenter().y);
    }
    //the live buttons
    for(int i = 0;i < liveBTNs.size();i++)
    {
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
    
    for(int i = 0;i < mirrors.size();i++)
    {
        mirrors[i].drawPreview(preTex.getTexture());
    }
    
    mirrors[0].getFbo(0).draw(ofGetWidth()-150,ofGetHeight()-50,150,50);
    masterBrightness->draw();
    ofDrawRectangle(buttons[0]);
    ofDrawRectangle(buttons[1]);
    
    ofDrawBitmapString("fps " + ofToString(ofGetFrameRate()),0,20);

}

void ofApp::setEditorID(int index)
{
    // first disable the old selection
    patEditors[editSelect]->setActive(false);
    editSelect = index;
    patEditors[editSelect]->setActive(true);
}

void ofApp::setLiveID(int index)
{
    liveSelect = index;
    //LIVE = patEditors[liveSelect];
}

void ofApp::isTrigger(int &triggerIndex)
{
    cout << "trigger" << endl;
    if(triggerIndex == 0) // the pattern for segments 1= color
    {
        for(int i = 0;i < mirrors.size();i++)
        {
            if(patEditors[editSelect]->getMirrorPattern()[i] == true)
            {
                bool left = patEditors[editSelect]->getMirrorSubPattern(i)[0];
                bool top = patEditors[editSelect]->getMirrorSubPattern(i)[1];
                bool right = patEditors[editSelect]->getMirrorSubPattern(i)[2];
                bool bottom = patEditors[editSelect]->getMirrorSubPattern(i)[3];
                mirrors[i].setEnables(left,top,right,bottom);
                //cout << "mirror" << i << " : " << left << " " << top << " " << right << " " << bottom << endl;
            }
            else
            {
                mirrors[i].setEnables(false,false,false,false);
                //cout << "mirror" << i << " FALSE " << endl;
            }
        }

    }
    else if(triggerIndex == 1)
    {
        //we have color trigger, is been removed
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
        cout << "id " << id << " l1 " << l1 << " l2 " << l2 << endl;
        // depending on the id, set the mirror points
        mirrors[id].setUniverses(l1,l2,t1,t2,r1,r2,b1,b2);
        settings.popTag();
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'n')
    {
        //send artnet testwise
        artnet->sendTest();
        cout << "artnet sended" << endl;
    }
    if(key == 'r')//reload settings
    {
        loadPixelMapping();
        artnet->loadNodes();
    }
}

void ofApp::exit()
{
    savePatternEditorSettings();
    for(int i = 0;i < patEditors.size();i++)
    {
        delete patEditors[i];
    }
    //delete uControl;
    delete artnet;
    delete masterBrightness;
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    for(int i = 0;i < previewBTNs.size();i++)
    {
        if(previewBTNs[i].inside(x, y))
        {
            setEditorID(i);
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
        cout << "flash" << endl;
    }
    if(buttons[1].inside(x,y))
    {
        //flash
        cout << "invert" << endl;
    }

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
