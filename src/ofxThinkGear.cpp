#include "ofxThinkGear.h"


ofxThinkGear::ofxThinkGear() {
    reset();
}

//--------------------------------------------------------------
ofxThinkGear::~ofxThinkGear(){}

//--------------------------------------------------------------
void ofxThinkGear::reset(){
    thinkGearBundle = NULL;
    TG_GetDriverVersion = NULL;
    TG_GetNewConnectionId = NULL;
    TG_ReadPackets = NULL;
    TG_GetValue = NULL;
    TG_GetValueStatus = NULL;
    TG_Disconnect = NULL;
    TG_FreeConnection = NULL;
    TG_EnableBlinkDetection = NULL;
    autoReading = false;
    isConnected = false;
    prevBlinkTime = 0;
    bEnableBlinkAsClick = false;
    newInfo = false;
    ableToConnect = false;
    connectionID = -1;
}

//--------------------------------------------------------------
void ofxThinkGear::setup(string deviceName) {
    // Defaults:
    // deviceName = "/dev/tty.MindWaveMobile-DevA";
    
    
#ifdef __APPLE__
    
    /* * *
    
     
     I wanted to build this in a way where you wouldn't have to import the bundle to your xcode project....all you'd have to do is add the addon.
     
     This seems hacky and I'm sure there is a better way to do this, but oh well.
     
     I was having trouble with discrepancies between xcode's working directory and the openFramweorks working directory when trying to load the ThinkGear.bundle. I found the following soulution here: http://stackoverflow.com/a/520951
     
     * * */
    
    
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
    }
    CFRelease(resourcesURL);
    chdir(path);
    std::cout << "Current Path: " << path << std::endl;
    
    
    /* * *
     
     This makes sure that the working directory is your .app's "Resources" folder. (This is seperate from setting the location of the openFrameworks data directory)
     
     Assuming your app is still in the bin folder, and your project is still in the OF file hierarchy, it then finds the ThinkGear.bundle in OF_ROOT/addons/ofxThinkGear/lib/ directory by using "../../../../../../../addons/ofxThinkGear/lib/ThinkGear.bundle"
     
     
     If you want to make the app portable, the ThinkGear.bundle needs to travel with your app.
     
     First, copy ThinkGear.bundle into your bin/data directory.
     
     In Xcode, you have to add the build script below to your xcode project, which will copy the data directory into the .app's Resources directory:
     
         cp -r bin/data/ThinkGear.bundle "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources";
     
     Then #define OF_RELEASE in your header, or add OF_RELEASE as a preprocessor macro in your build settings
     
     * * */
    
    #ifdef OF_RELEASE
        bundleURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                                  CFSTR("data/ThinkGear.bundle"),
                                                  kCFURLPOSIXPathStyle,
                                                  true);
    #else
        bundleURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                                  CFSTR("../../../../../../../addons/ofxThinkGear/lib/ThinkGear.bundle"),
                                                  kCFURLPOSIXPathStyle,
                                                  true);
    #endif
#endif
    
    //cout << "curr dir: " << ofFilePath::getCurrentWorkingDirectory() << endl;
    
    thinkGearBundle = CFBundleCreate(kCFAllocatorDefault, bundleURL);
    
    if (!thinkGearBundle)
    {
        ofLog(OF_LOG_FATAL_ERROR) << "Error: Could not find ThinkGear.bundle. Does it exist in the current directory?";
        if (errorExit) {
            exit(1);
        }
        return;
    }
    
    // set function pointers
    TG_GetDriverVersion = (int(*)())CFBundleGetFunctionPointerForName(thinkGearBundle, CFSTR("TG_GetDriverVersion"));
    TG_GetNewConnectionId = (int(*)())CFBundleGetFunctionPointerForName(thinkGearBundle, CFSTR("TG_GetNewConnectionId"));
    TG_Connect = (int(*)(int, const char*, int, int))CFBundleGetFunctionPointerForName(thinkGearBundle, CFSTR("TG_Connect"));
    TG_ReadPackets = (int(*)(int, int))CFBundleGetFunctionPointerForName(thinkGearBundle, CFSTR("TG_ReadPackets"));
    TG_GetValue = (float(*)(int, int))CFBundleGetFunctionPointerForName(thinkGearBundle, CFSTR("TG_GetValue"));
    TG_GetValueStatus = (bool(*)(int, int))CFBundleGetFunctionPointerForName(thinkGearBundle, CFSTR("TG_GetValueStatus"));
    TG_Disconnect = (int(*)(int))CFBundleGetFunctionPointerForName(thinkGearBundle, CFSTR("TG_Disconnect"));
    TG_FreeConnection = (void(*)(int))CFBundleGetFunctionPointerForName(thinkGearBundle, CFSTR("TG_FreeConnection"));
    TG_EnableBlinkDetection = (int(*)(int, int))CFBundleGetFunctionPointerForName(thinkGearBundle, CFSTR("TG_EnableBlinkDetection"));
    TG_EnableAutoRead = (int(*)(int, int))CFBundleGetFunctionPointerForName(thinkGearBundle, CFSTR("TG_EnableAutoRead"));
    
    if (!TG_GetDriverVersion ||
        !TG_GetNewConnectionId ||
        !TG_Connect ||
        !TG_ReadPackets ||
        !TG_GetValue ||
        !TG_Disconnect ||
        !TG_FreeConnection ||
        !TG_EnableBlinkDetection ||
        !TG_EnableAutoRead)
    {
        ofLog(OF_LOG_FATAL_ERROR) << "Error: Expected functions in ThinkGear.bundle were not found.";
        if (errorExit) {
            exit(1);
        }
        return;
    }
    
    connectionID = TG_GetNewConnectionId();
    
    ofLog() << "Connecting to " << deviceName << " ...";
    
    int conResult = TG_Connect(connectionID, deviceName.c_str(), TG_BAUDRATE, TG_STREAM_PACKETS);
    
    switch (conResult) {
        case 0:
            ofLog() << "Connected to ID: " << connectionID << " at: " << deviceName << " ...";
            ableToConnect = true;
            isConnected = true;
            break;
        case -1:
            ofLog(OF_LOG_FATAL_ERROR) << "\"connectionId\" does not refer to a valid ThinkGear Connection ID handle.";
            if (errorExit) {
                exit(1);
            }
            break;
        case -2:
            ofLog(OF_LOG_FATAL_ERROR) << deviceName << " could not be opened. Check that the name is a valid COM port on your system.";
            if (errorExit) {
                exit(1);
            }
            break;
        case -3:
            ofLog(OF_LOG_FATAL_ERROR) << "Baudrate is not a valid TG_BAUD_* value.";
            if (errorExit) {
                exit(1);
            }
            break;
        case -4:
            ofLog(OF_LOG_FATAL_ERROR) << "serialDataFormat is not a valid TG_STREAM_* type";
            if (errorExit) {
                exit(1);
            }
            break;
        default:
            ofLog(OF_LOG_FATAL_ERROR) << "Error: ThinkGear connection failed.";
            if (errorExit) {
                exit(1);
            }
            break;
    }
    
    int resBlink = TG_EnableBlinkDetection(connectionID, 1);
    if (resBlink != 0)
        ofLog() << "fail to enable blink detection";
}

//--------------------------------------------------------------
void ofxThinkGear::update(){
    
    if (!autoReading) {
        // From API:
        // "While background auto-reading is enabled, the TG_GetValueStatus() function
        // is pretty much useless. Also, the TG_ReadPackets() function should probably not be called."
        
        int numPackets = TG_ReadPackets(connectionID, -1);
        
        if (numPackets <= 0) {  /// <-------- changed to <=
            newInfo = false;
        }
        if (numPackets > 0)
        {
            //newInfo = true;
            signalQuality = TG_GetValue(connectionID, TG_DATA_POOR_SIGNAL);
            
            if (signalQuality == 0.0)
            {
                /*
                 reminder: values array is:
                 
                 values[0] is Attention
                 values[1] is Meditation
                 values[2] is Delta
                 values[3] is Theta
                 values[4] is Alpha 1
                 values[5] is Alpha 2
                 values[6] is Beta 1
                 values[7] is Beta 2
                 values[8] is Gamma 1
                 values[9] is Gamma 2
                 */
                //////////
                
                newInfo = true;
                
                if (TG_GetValueStatus(connectionID, TG_DATA_ATTENTION) != 0.0)
                {
                    //float attention = TG_GetValue(connectionID, TG_DATA_ATTENTION);
                    values[0] = TG_GetValue(connectionID, TG_DATA_ATTENTION);
                    ofNotifyEvent(attentionChangeEvent, values[0]);
                }
                //////////
                if (TG_GetValueStatus(connectionID, TG_DATA_MEDITATION) != 0.0)
                {
                    //float meditation = TG_GetValue(connectionID, TG_DATA_MEDITATION);
                    values[1] = TG_GetValue(connectionID, TG_DATA_MEDITATION);
                    ofNotifyEvent(meditationChangeEvent, values[1]);
                }
                ////////// Events can be added for any of the available data points
                if (TG_GetValueStatus(connectionID, TG_DATA_DELTA) != 0.0)
                {
                    values[2] = TG_GetValue(connectionID, TG_DATA_DELTA);
                    ofNotifyEvent(deltaChangeEvent, values[2]);
                }
                //////////
                if (TG_GetValueStatus(connectionID, TG_DATA_THETA) != 0.0)
                {
                    values[3] = TG_GetValue(connectionID, TG_DATA_THETA);
                    ofNotifyEvent(thetaChangeEvent, values[3]);
                }
                //////////
                if (TG_GetValueStatus(connectionID, TG_DATA_ALPHA1) != 0.0)
                {
                    values[4] = TG_GetValue(connectionID, TG_DATA_ALPHA1);
                    ofNotifyEvent(alpha1ChangeEvent, values[4]);
                }
                //////////
                if (TG_GetValueStatus(connectionID, TG_DATA_ALPHA2) != 0.0)
                {
                    values[5] = TG_GetValue(connectionID, TG_DATA_ALPHA2);
                    ofNotifyEvent(alpha2ChangeEvent, values[5]);
                }
                //////////
                if (TG_GetValueStatus(connectionID, TG_DATA_BETA1) != 0.0)
                {
                    values[6] = TG_GetValue(connectionID, TG_DATA_BETA1);
                    ofNotifyEvent(beta1ChangeEvent, values[6]);
                }
                //////////
                if (TG_GetValueStatus(connectionID, TG_DATA_BETA2) != 0.0)
                {
                    values[7] = TG_GetValue(connectionID, TG_DATA_BETA2);
                    ofNotifyEvent(beta2ChangeEvent, values[7]);
                }
                //////////
                if (TG_GetValueStatus(connectionID, TG_DATA_GAMMA1) != 0.0)
                {
                    values[8] = TG_GetValue(connectionID, TG_DATA_GAMMA1);
                    ofNotifyEvent(gamma1ChangeEvent, values[8]);
                }
                //////////
                if (TG_GetValueStatus(connectionID, TG_DATA_GAMMA2) != 0.0)
                {
                    values[9] = TG_GetValue(connectionID, TG_DATA_GAMMA2);
                    ofNotifyEvent(gamma2ChangeEvent, values[9]);
                }
                //////////
                
                //////////
                if (TG_GetValueStatus(connectionID, TG_DATA_BLINK_STRENGTH) != 0.0)
                {
                    float blink = TG_GetValue(connectionID, TG_DATA_BLINK_STRENGTH);
                    ofNotifyEvent(blinkChangeEvent, blink);
                    
                    if (bEnableBlinkAsClick)
                    {
                        bool fake = true;
                        
                        if (ofGetElapsedTimeMillis() - prevBlinkTime < 500)
                        {
                            ofNotifyEvent(doubleClickEvent, fake);
                        }
                        else
                        {
                            ofNotifyEvent(singleClickEvent, fake);
                        }
                        prevBlinkTime = ofGetElapsedTimeMillis();
                    }
                }
            } else {
                newInfo = false;
            }
        }
    } else {
        cout << "Autoreading is enabled! Use autoUpdate() instead." << endl;
    }
}

//--------------------------------------------------------------
void ofxThinkGear::autoUpdate(){
    
    if (autoReading) {
        // From API:
        // "While background auto-reading is enabled, the TG_GetValueStatus() function
        // is pretty much useless. Also, the TG_ReadPackets() function should probably not be called."
        
        signalQuality = TG_GetValue(connectionID, TG_DATA_POOR_SIGNAL);
        
        cout << signalQuality << endl;
        
        if (signalQuality == 0.0)
        {
            
            /*
             reminder: values array is:
             
             values[0] is Attention
             values[1] is Meditation
             values[2] is Delta
             values[3] is Theta
             values[4] is Alpha 1
             values[5] is Alpha 2
             values[6] is Beta 1
             values[7] is Beta 2
             values[8] is Gamma 1
             values[9] is Gamma 2
             */
            
            
            /* IN PROGRESS
             
             values[0] = TG_GetValue(connectionID, TG_DATA_ATTENTION);
             values[1] = TG_GetValue(connectionID, TG_DATA_MEDITATION);
             
             for (int i=2; i<10; i++) {
             values[i] = TG_GetValue(connectionID, i+3);
             }
             */
        }
    } else {
        cout << "Autoreading is disabled! Use update() instead." << endl;
    }
}

//--------------------------------------------------------------
void ofxThinkGear::startAutoRead() {
    if (isConnected){
        int aReadStatus = TG_EnableAutoRead(connectionID,1);
        switch (aReadStatus) {
            case -1:
                ofLog() << "\"connectionId\" does not refer to a valid ThinkGear Connection ID handle";
                break;
            case -2:
                ofLog() << "Unable to enable background auto-reading";
                break;
            case -3:
                ofLog() << "An error occured while attempting to disable background auto-reading";
                break;
            default:
                ofLog() << "Started AUTOREAD on connection ID: " << connectionID;
                autoReading = true;
                break;
        }
    } else {
        ofLog() << "There is no active connection!";
    }
}

//--------------------------------------------------------------
void ofxThinkGear::stopAutoRead() {
    int aReadStatus = TG_EnableAutoRead(connectionID,0);
    switch (aReadStatus) {
        case -1:
            ofLog() << "\"connectionId\" does not refer to a valid ThinkGear Connection ID handle";
            break;
        case -2:
            ofLog() << "Unable to enable background auto-reading";
            break;
        case -3:
            ofLog() << "An error occured while attempting to disable background auto-reading";
            break;
        default:
            ofLog() << "Stopped AUTOREAD on connection ID: " << connectionID;
            autoReading = false;
            break;
    }
}

//--------------------------------------------------------------
void ofxThinkGear::enableBlinkAsClick() {
    bEnableBlinkAsClick = true;
}

//--------------------------------------------------------------
void ofxThinkGear::disableBlinkAsClick() {
    bEnableBlinkAsClick = false;
}

//--------------------------------------------------------------
float ofxThinkGear::getSignalQuality() {
    if (isConnected) {
        return signalQuality;
    } else {
        return 300.0;
    }
}

//--------------------------------------------------------------
int ofxThinkGear::getID() {
    return connectionID;
}

//--------------------------------------------------------------
bool ofxThinkGear::getIsConnected() {
    return isConnected;
}

//--------------------------------------------------------------
bool ofxThinkGear::getNewInfo() {
    return newInfo;
}

//--------------------------------------------------------------
void ofxThinkGear::freeConnection(){
    if (isConnected) {
        TG_FreeConnection(connectionID);
        ofLog() << "disconnecting connection ID: " << connectionID;
        isConnected = false;
        cout << "freed connection" << endl;
    }
}

//--------------------------------------------------------------

//--------------------------------------------------------------

//--------------------------------------------------------------

//--------------------------------------------------------------

//--------------------------------------------------------------

