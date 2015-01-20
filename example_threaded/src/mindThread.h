#pragma once

#include "ofMain.h"
#include "ofThread.h"
#include "ofxThinkGear.h"


class MindThread: public ofThread {

public:
    
    MindThread();
    ~MindThread();
    void connect();
    void start();
    void stop();
    void threadedFunction();
    void draw();
    int getCount();
    
private:
    ofxThinkGear thinkGear;
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
    
    bool isConnected;
    
protected:
    // This is a simple variable that we aim to always access from both the
    // main thread AND this threaded object.  Therefore, we need to protect it
    // with the mutex.  In the case of simple numerical variables, some
    // garuntee thread safety for small integral types, but for the sake of
    // illustration, we use an int.  This int could represent ANY complex data
    // type that needs to be protected.
    int count;
    float values[10];
    
};
