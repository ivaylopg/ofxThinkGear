#pragma once

#include "ofMain.h"
#include "ofxThinkGear.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void exit();
    
    vector<ofVec2f> meds;
    vector<ofVec2f> atts;
    
    float values[10];
    float averages[10];
    int lastBlinkVal;
    int numBlinks;
		
    
    private:
        ofxThinkGear thinkGear;
        void blinkListener(float &param);
};
