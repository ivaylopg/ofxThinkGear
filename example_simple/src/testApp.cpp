#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    thinkGear.setup();
    ofAddListener(thinkGear.attentionChangeEvent, this, &testApp::attentionListener);
    ofAddListener(thinkGear.meditationChangeEvent, this, &testApp::meditationListener);
    ofAddListener(thinkGear.blinkChangeEvent, this, &testApp::blinkListener);
    
    
    ofAddListener(thinkGear.deltaChangeEvent, this, &testApp::deltaListener);
    ofAddListener(thinkGear.thetaChangeEvent, this, &testApp::thetaListener);
    ofAddListener(thinkGear.alpha1ChangeEvent, this, &testApp::alpha1Listener);
    ofAddListener(thinkGear.alpha2ChangeEvent, this, &testApp::alpha2Listener);
    ofAddListener(thinkGear.beta1ChangeEvent, this, &testApp::beta1Listener);
    ofAddListener(thinkGear.beta2ChangeEvent, this, &testApp::beta2Listener);
    ofAddListener(thinkGear.gamma1ChangeEvent, this, &testApp::gamma1Listener);
    ofAddListener(thinkGear.gamma2ChangeEvent, this, &testApp::gamma2Listener);
    
    /*
     All of the available data points triggger ofEvents when they are updated. You only
     need to declare and use the ones you need (usually just attention and meditation)
     
     Even though you are calling thinkGear.update() every time through testApp::update(),
     you will only get an event when the headset has new data, which is about once
     a second. 
     
     This also ensures you are not reassigning old data to a variable or, in this
     example, the graph will only grow when the data is updated.
     
     Make sure you call thinkGear.freeConnection() in testApp::exit() to properly 
     close the connection to the headset.
     */
    
    attention = 0.0;
    meditation = 0.0;
    numBlinks = 0;
    delta = 0.0;
    theta = 0.0;
    alpha1 = 0.0;
    alpha2 = 0.0;
    beta1 = 0.0;
    beta2 = 0.0;
    gamma1 = 0.0;
    gamma2 = 0.0;

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
    
    if (thinkGear.getSignalQuality() == 300.0) {
        qStr = "there is no headset connected!";
    } else if (thinkGear.getSignalQuality() == 0.0) {
        qStr = "good";
    } else {
        qStr = "poor (" + ofToString(thinkGear.getSignalQuality()) + ")";
    }
    
    ofDrawBitmapString("Signal Quality: " + qStr, 20,20);
    ofDrawBitmapString("Number of Blinks: " + ofToString(numBlinks) + " [" + ofToString(lastBlinkVal) + "]", 20,40);
    
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
    
    
    ofSetColor(255);
    ofDrawBitmapString("Delta: " +      ofToString(delta,3) +
                       "\nTheta: " +      ofToString(theta,3) +
                       "\nAlpha 1: " +    ofToString(alpha1,3) +
                       "\nAlpha 2: " +    ofToString(alpha2,3) +
                       "\nBeta 1: " +     ofToString(beta1,3) +
                       "\nBeta 2: " +     ofToString(beta2,3) +
                       "\nGamma 1: " +    ofToString(gamma1,3) +
                       "\nGamma 2: " +    ofToString(gamma2,3)
                       , 20, ofGetHeight()-150);
    
}

//--------------------------------------------------------------
void testApp::attentionListener(float &param)
{
    attention = param;
    for (int i=0; i < atts.size(); i++) {
        atts[i].x-=32;
    }
    if (atts.size()>32) {
        atts.erase(atts.begin());
    }
    ofVec2f vA = ofVec2f(ofGetWidth(),attention);
    atts.push_back(vA);
    
}

//--------------------------------------------------------------
void testApp::meditationListener(float &param)
{
    meditation = param;
    for (int i=0; i < meds.size(); i++) {
        meds[i].x-=32;
    }
    if (meds.size()>32) {
        meds.erase(meds.begin());
    }
    ofVec2f vM = ofVec2f(ofGetWidth(),meditation);
    meds.push_back(vM);
}

//--------------------------------------------------------------
void testApp::blinkListener(float &param)
{
    numBlinks++;
    lastBlinkVal = param;
}

//--------------------------------------------------------------
void testApp::deltaListener(float &param)
{
    delta = param;
}

//--------------------------------------------------------------
void testApp::thetaListener(float &param)
{
    theta = param;
}

//--------------------------------------------------------------
void testApp::alpha1Listener(float &param)
{
    alpha1 = param;
}

//--------------------------------------------------------------
void testApp::alpha2Listener(float &param)
{
    alpha2 = param;
}

//--------------------------------------------------------------
void testApp::beta1Listener(float &param)
{
    beta1 = param;
}

//--------------------------------------------------------------
void testApp::beta2Listener(float &param)
{
    beta2 = param;
}

//--------------------------------------------------------------
void testApp::gamma1Listener(float &param)
{
    gamma1 = param;
}

//--------------------------------------------------------------
void testApp::gamma2Listener(float &param)
{
    gamma2 = param;
}

//--------------------------------------------------------------
void testApp::exit(){
    thinkGear.freeConnection();
}
