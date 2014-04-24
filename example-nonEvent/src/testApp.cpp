#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    thinkGear.setup();
    ofAddListener(thinkGear.blinkChangeEvent, this, &testApp::blinkListener);
    
    for (int i=0; i < 10; i++) {
        values[i] = 0;
        averages[i] = 0.0;
    }
    
    numBlinks = 0;
    lastBlinkVal = 0;

}

//--------------------------------------------------------------
void testApp::update(){
    
    /*
     values array is:
     
     values[0] is TG_DATA_ATTENTION = 2;
     values[1] is TG_DATA_MEDITATION = 3;
     values[2] is TG_DATA_DELTA = 5;
     values[3] is TG_DATA_THETA = 6;
     values[4] is TG_DATA_ALPHA1 = 7;
     values[5] is TG_DATA_ALPHA2 = 8;
     values[6] is TG_DATA_BETA1 = 9;
     values[7] is TG_DATA_BETA2 = 10;
     values[8] is TG_DATA_GAMMA1 = 11;
     values[9] is TG_DATA_GAMMA2 = 12;
     */
    
    //thinkGear.update();
    
    if (ofGetFrameNum()%60 == 0) {
        
        thinkGear.update();
        
        for (int i=0; i<10; i++) {
            values[i] = thinkGear.values[i];
            
        }
        
        // Update attention
        for (int i=0; i < atts.size(); i++) {
            atts[i].x-=32;
        }
        if (atts.size()>32) {
            atts.erase(atts.begin());
        }
        ofVec2f vA = ofVec2f(ofGetWidth(),values[0]);
        atts.push_back(vA);
        
        
        // Update meditation
        for (int i=0; i < meds.size(); i++) {
            meds[i].x-=32;
        }
        if (meds.size()>32) {
            meds.erase(meds.begin());
        }
        ofVec2f vM = ofVec2f(ofGetWidth(),values[1]);
        meds.push_back(vM);
        
        // CSV
        // cout << values[1] << ",";
    }
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
    ofDrawBitmapString("Number of Blinks: " + ofToString(numBlinks) + " [" + ofToString(lastBlinkVal) + "]", 20,40);
    
    for (int i=0; i < 10; i++) {
        averages[i] = 0.0;
    }
    
    ofSetColor(255);
    ofDrawBitmapString("Attention: " + ofToString(values[0]), 20, 220);
    for (int i=0; i < atts.size(); i++) {
        ofSetRectMode(OF_RECTMODE_CORNER); //set rectangle mode to the corner
        int base = 200;
        averages[0] += atts[i].y;
        ofRect(atts[i].x,base-atts[i].y,30,atts[i].y);
    }
    
    if (atts.size() > 0) {
        ofSetColor(255, 0, 0);
        int aH = averages[0]/atts.size();
        ofLine(0, 200-aH, ofGetWidth(), 200-aH);
        ofDrawBitmapString("Average: " + ofToString(aH), 200, 220);
    }
    
    
    
    ofSetColor(255);
    ofDrawBitmapString("Meditation: " + ofToString(values[1]), 20, 380);
    for (int i=0; i < meds.size(); i++) {
        ofSetRectMode(OF_RECTMODE_CORNER); //set rectangle mode to the corner
        int base = 360;
        averages[1] += meds[i].y;
        ofRect(meds[i].x,base-meds[i].y,30,meds[i].y);
    }
    
    if (meds.size() > 0) {
        ofSetColor(255, 0, 0);
        int aH = averages[1]/meds.size();
        ofLine(0, 360-aH, ofGetWidth(), 360-aH);
        ofDrawBitmapString("Average: " + ofToString(aH), 200, 380);
    }
    
    
    // All Values:
    ofSetColor(255);
    ofDrawBitmapString("Attention: " +    ofToString(values[0],3) +
                       "\nMeditation: " + ofToString(values[1],3) +
                       "\nDelta: " +      ofToString(values[2],3) +
                       "\nTheta: " +      ofToString(values[3],3) +
                       "\nAlpha 1: " +    ofToString(values[4],3) +
                       "\nAlpha 2: " +    ofToString(values[5],3) +
                       "\nBeta 1: " +     ofToString(values[6],3) +
                       "\nBeta 2: " +     ofToString(values[7],3) +
                       "\nGamma 1: " +    ofToString(values[8],3) +
                       "\nGamma 2: " +    ofToString(values[9],3)
                       , 20, ofGetHeight()-250);

    
    
    
    
    
}

//--------------------------------------------------------------
void testApp::blinkListener(float &param)
{
    numBlinks++;
    lastBlinkVal = param;
    //cout << param << endl;
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
//--------------------------------------------------------------
void testApp::exit(){
    thinkGear.freeConnection();
}
