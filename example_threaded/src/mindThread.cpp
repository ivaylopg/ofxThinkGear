#include "mindThread.h"


MindThread::MindThread() {
    count = 0;
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
            // The mutex is now locked and the "count"
            // variable is protected.  Time to modify it.
            count++;
            
            // Here, we simply cause it to reset to zero if it gets big.
            if(count > 50000) count = 0;
            
            // Unlock the mutex.  This is only
            // called if lock() returned true above.
            unlock();
            
            // Sleep for 1 second.
            sleep(1000);
        }
        else
        {
            // If we reach this else statement, it means that we could not
            // lock our mutex, and so we do not need to call unlock().
            // Calling unlock without locking will lead to problems.
            
            ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
        }
    }
}

//--------------------------------------------------------------
void MindThread::draw() {
    /// This drawing function cannot be called from the thread itself because
    /// it includes OpenGL calls (ofDrawBitmapString).
    
    std::stringstream ss;
    
    ss << "I am a slowly increasing thread. " << std::endl;
    ss << "My current count is: ";
    
    if(lock())
    {
        // The mutex is now locked and the "count"
        // variable is protected.  Time to read it.
        ss << count;
        
        // Unlock the mutex.  This is only
        // called if lock() returned true above.
        unlock();
    }
    else
    {
        // If we reach this else statement, it means that we could not
        // lock our mutex, and so we do not need to call unlock().
        // Calling unlock without locking will lead to problems.
        //ofLogWarning("threadedFunction()") << "Unable to lock mutex.";
    }
    
    ofDrawBitmapString(ss.str(), 50, 56);
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