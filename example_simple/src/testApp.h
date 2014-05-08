#pragma once

#include "ofMain.h"
#include "ofxThinkGear.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

        void exit();
    
    vector<ofVec2f> meds;
    vector<ofVec2f> atts;
    int avgAtt;
    int avgMed;
		
    
    private:
        ofxThinkGear thinkGear;
        void attentionListener(float &param);
        void meditationListener(float &param);
        void blinkListener(float &param);
    
        void deltaListener(float &param);
        void thetaListener(float &param);
        void alpha1Listener(float &param);
        void alpha2Listener(float &param);
        void beta1Listener(float &param);
        void beta2Listener(float &param);
        void gamma1Listener(float &param);
        void gamma2Listener(float &param);
    
        float attention;
        float meditation;
    
        float delta;
        float theta;
        float alpha1;
        float alpha2;
        float beta1;
        float beta2;
        float gamma1;
        float gamma2;
    
        int numBlinks;
        int lastBlinkVal;
};
