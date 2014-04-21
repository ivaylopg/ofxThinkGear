#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    thinkGear.setup();
    ofAddListener(thinkGear.attentionChangeEvent, this, &testApp::attentionListener);
    ofAddListener(thinkGear.meditationChangeEvent, this, &testApp::meditationListener);
    ofAddListener(thinkGear.blinkChangeEvent, this, &testApp::blinkListener);
    
    attention = 0.0;
    meditation = 0.0;
    numBlinks = 0;

}

//--------------------------------------------------------------
void testApp::update(){
    thinkGear.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    ofSetColor(255);
    string qStr = "";
    if (thinkGear.getSignalQuality() == 0.0)
    {
        qStr = "good";
    }
    else
    {
        qStr = "poor (" + ofToString(thinkGear.getSignalQuality()) + ")";
    }
    
    ofDrawBitmapString("Signal Quality: " + qStr, 20,20);
    ofDrawBitmapString("Number of Blinks: " + ofToString(numBlinks), 20,40);
    
    avgAtt = 0;
    avgMed = 0;
    
    ofDrawBitmapString("Attention: " + ofToString(attention), 20, 220);
    for (int i=0; i < atts.size(); i++) {
        ofSetRectMode(OF_RECTMODE_CORNER); //set rectangle mode to the corner
        int base = 200;
        avgAtt += atts[i].y;
        ofRect(atts[i].x,base-atts[i].y,30,atts[i].y);
    }
    
    
    ofDrawBitmapString("Meditation: " + ofToString(meditation), 20, 380);
    for (int i=0; i < meds.size(); i++) {
        ofSetRectMode(OF_RECTMODE_CORNER); //set rectangle mode to the corner
        int base = 360;
        avgMed += meds[i].y;
        ofRect(meds[i].x,base-meds[i].y,30,meds[i].y);
    }
    
    if (atts.size() > 0) {
        ofSetColor(255, 0, 0);
        int aH = avgAtt/atts.size();
        ofLine(0, 200-aH, ofGetWidth(), 200-aH);
        ofDrawBitmapString("Average: " + ofToString(aH), 200, 220);
    }
    
    if (meds.size() > 0) {
        ofSetColor(255, 0, 0);
        int aH = avgMed/meds.size();
        ofLine(0, 360-aH, ofGetWidth(), 360-aH);
        ofDrawBitmapString("Average: " + ofToString(aH), 200, 380);
    }
    
    
    
    
    
    
}

//--------------------------------------------------------------
void testApp::attentionListener(float &param)
{
    attention = param;
    for (int i=0; i < atts.size(); i++) {
        atts[i].x+=32;
    }
    if (atts.size()>32) {
        atts.erase(atts.begin());
    }
    ofVec2f vA = ofVec2f(0,attention);
    atts.push_back(vA);
    
}

//--------------------------------------------------------------
void testApp::meditationListener(float &param)
{
    meditation = param;
    for (int i=0; i < meds.size(); i++) {
        meds[i].x+=32;
    }
    if (meds.size()>32) {
        meds.erase(meds.begin());
    }
    ofVec2f vM = ofVec2f(0,meditation);
    meds.push_back(vM);
}

//--------------------------------------------------------------
void testApp::blinkListener(float &param)
{
    numBlinks++;
    cout << param << endl;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
