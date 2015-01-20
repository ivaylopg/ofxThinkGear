#include "mindThread.h"


MindThread::MindThread() {
    count = 0;
    
    attention = 0.0;
    meditation = 0.0;
    delta = 0.0;
    theta = 0.0;
    alpha1 = 0.0;
    alpha2 = 0.0;
    beta1 = 0.0;
    beta2 = 0.0;
    gamma1 = 0.0;
    gamma2 = 0.0;
    
    for (int i = 0; i < 10; i++) {
        values[i] = 0.0;
    }
    
    isConnected = false;
}

//--------------------------------------------------------------
MindThread::~MindThread() {
    //cout << "dead" << endl;
    thinkGear.freeConnection();
}

//--------------------------------------------------------------
void MindThread::connect() {
    if (!isConnected) {
        thinkGear.setup();
        isConnected = true;
    }
}

//--------------------------------------------------------------
void MindThread::start() {
    startThread();
}

//--------------------------------------------------------------
void MindThread::stop() {
    stopThread();
}

//--------------------------------------------------------------
void MindThread::threadedFunction() {
    while(isThreadRunning()) {
        // Attempt to lock the mutex.  If blocking is turned on,
        if(lock())
        {
            // The mutex is now locked and the variables are protected.
            
            

            
            // Unlock the mutex.  This is only called if lock() returned true above.
            unlock();
            sleep(1000);
        }
        else
        {
            ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
        }
    }
}

//--------------------------------------------------------------
void MindThread::draw() {
    
}

//--------------------------------------------------------------
int MindThread::getCount() {
    // Use ofScopedLock to protect a copy of count while getting a copy.
    ofScopedLock lock(mutex);
    return count;
}

//--------------------------------------------------------------

//--------------------------------------------------------------

//--------------------------------------------------------------