#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    thinkGear.setup();
    ofAddListener(thinkGear.attentionChangeEvent, this, &ofApp::attentionListener);
    ofAddListener(thinkGear.meditationChangeEvent, this, &ofApp::meditationListener);
    ofAddListener(thinkGear.blinkChangeEvent, this, &ofApp::blinkListener);
    
    
    ofAddListener(thinkGear.deltaChangeEvent, this, &ofApp::deltaListener);
    ofAddListener(thinkGear.thetaChangeEvent, this, &ofApp::thetaListener);
    ofAddListener(thinkGear.alpha1ChangeEvent, this, &ofApp::alpha1Listener);
    ofAddListener(thinkGear.alpha2ChangeEvent, this, &ofApp::alpha2Listener);
    ofAddListener(thinkGear.beta1ChangeEvent, this, &ofApp::beta1Listener);
    ofAddListener(thinkGear.beta2ChangeEvent, this, &ofApp::beta2Listener);
    ofAddListener(thinkGear.gamma1ChangeEvent, this, &ofApp::gamma1Listener);
    ofAddListener(thinkGear.gamma2ChangeEvent, this, &ofApp::gamma2Listener);
    
    /*
     All of the available data points triggger ofEvents when they are updated. You only 
     need to declare listeners for the ones you need (usually just attention and meditation).
     
     Even though you are calling thinkGear.update() every time through ofApp::update(), 
     you will only get an event when the headset has new data, which is about once a second.
     
     This also ensures you are not reassigning old data to a variable. In this example, 
     the graph will only grow when the data is updated.
     
     Make sure you call thinkGear.freeConnection() in ofApp::exit() to properly 
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
    
    thinkGear.startAutoRead();

}

//--------------------------------------------------------------
void ofApp::update(){
    //thinkGear.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
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
void ofApp::attentionListener(float &param)
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
void ofApp::meditationListener(float &param)
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
void ofApp::blinkListener(float &param)
{
    numBlinks++;
    lastBlinkVal = param;
}

//--------------------------------------------------------------
void ofApp::deltaListener(float &param)
{
    delta = param;
}

//--------------------------------------------------------------
void ofApp::thetaListener(float &param)
{
    theta = param;
}

//--------------------------------------------------------------
void ofApp::alpha1Listener(float &param)
{
    alpha1 = param;
}

//--------------------------------------------------------------
void ofApp::alpha2Listener(float &param)
{
    alpha2 = param;
}

//--------------------------------------------------------------
void ofApp::beta1Listener(float &param)
{
    beta1 = param;
}

//--------------------------------------------------------------
void ofApp::beta2Listener(float &param)
{
    beta2 = param;
}

//--------------------------------------------------------------
void ofApp::gamma1Listener(float &param)
{
    gamma1 = param;
}

//--------------------------------------------------------------
void ofApp::gamma2Listener(float &param)
{
    gamma2 = param;
}

//--------------------------------------------------------------
void ofApp::exit(){
    thinkGear.freeConnection();
}
