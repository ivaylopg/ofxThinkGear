#pragma once

#include "ofMain.h"
#include "ofThread.h"


class MindThread: public ofThread {

public:
    
    MindThread();
    void start();
    void stop();
    void threadedFunction();
    void draw();
    int getCount();
    
protected:
    // This is a simple variable that we aim to always access from both the
    // main thread AND this threaded object.  Therefore, we need to protect it
    // with the mutex.  In the case of simple numerical variables, some
    // garuntee thread safety for small integral types, but for the sake of
    // illustration, we use an int.  This int could represent ANY complex data
    // type that needs to be protected.
    int count;
    
};
