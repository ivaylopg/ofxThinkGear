//
//  ofxThinkGear.h
//  ofxThinkGear
//
//  Created by Akira Hayasaka on 8/24/12.
//  Copyright (c) 2012 All rights reserved.
//
//
//  Forked repo by Ivaylo Getov April 2014
//
//  Place ThinkGear.bundle inside "data" folder


#pragma once
#include "ofMain.h"

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

const int TG_BAUDRATE = 9600;
//const int TG_BAUDRATE = 57600;

// Data format for use with TG_Connect() and TG_SetDataFormat()
const int TG_STREAM_PACKETS = 0;
const int TG_STREAM_5VRAW = 1;
const int TG_STREAM_FILE_PACKETS = 2;

// Data type that can be requested from TG_GetValue()
const int TG_DATA_BATTERY = 0;
const int TG_DATA_POOR_SIGNAL = 1; // 0 = good. others = poor.
const int TG_DATA_ATTENTION = 2; // min = 0. max = 100.
const int TG_DATA_MEDITATION = 3; // min = 0. max = 100.
const int TG_DATA_RAW = 4;
const int TG_DATA_DELTA = 5;
const int TG_DATA_THETA = 6;
const int TG_DATA_ALPHA1 = 7;
const int TG_DATA_ALPHA2 = 8;
const int TG_DATA_BETA1 = 9;
const int TG_DATA_BETA2 = 10;
const int TG_DATA_GAMMA1 = 11;
const int TG_DATA_GAMMA2 = 12;
const int TG_DATA_BLINK_STRENGTH = 37; // min = 0. max = 255.



class ofxThinkGear
{
public:
    
    ofxThinkGear()
    {
        thinkGearBundle = NULL;
        TG_GetDriverVersion = NULL;
        TG_GetNewConnectionId = NULL;
        TG_ReadPackets = NULL;
        TG_GetValue = NULL;
        TG_GetValueStatus = NULL;
        TG_Disconnect = NULL;
        TG_FreeConnection = NULL;
        TG_EnableBlinkDetection = NULL;
        tgID = 0;
        autoReading = false;
        isConnected = false;
        prevBlinkTime = 0;
        bEnableBlinkAsClick = false;
    }
    
    void setup(string deviceName = "/dev/tty.MindWaveMobile-DevA", int _id = 0)
    {
        tgID = _id;
        
        //char curPath[1000];
        //cout << getcwd(curPath, 1000) << endl;
        //chdir(path);
        
#ifdef __APPLE__
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
#endif
        
        bundleURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                                  CFSTR("../../../data/ThinkGear.bundle"),
                                                  kCFURLPOSIXPathStyle,
                                                  true);
        
        
        thinkGearBundle = CFBundleCreate(kCFAllocatorDefault, bundleURL);
        
        if (!thinkGearBundle)
        {
            ofLog(OF_LOG_FATAL_ERROR) << "Error: Could not find ThinkGear.bundle. Does it exist in the current directory?";
            exit(1);
        }
        
        // set func ptrs
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
            exit(1);
        }
        
        connectionID = TG_GetNewConnectionId();
        
        ofLog() << "Connecting to " << deviceName << " ...";
        int conResult = TG_Connect(connectionID, deviceName.c_str(), TG_BAUDRATE, TG_STREAM_PACKETS);
        
        
        switch (conResult) {
            case -1:
                ofLog(OF_LOG_FATAL_ERROR) << "\"connectionId\" does not refer to a valid ThinkGear Connection ID handle.";
                exit(1);
                break;
            case -2:
                ofLog(OF_LOG_FATAL_ERROR) << deviceName << " could not be opened. Check that the name is a valid COM port on your system.";
                exit(1);
                break;
            case -3:
                ofLog(OF_LOG_FATAL_ERROR) << "Baudrate is not a valid TG_BAUD_* value.";
                exit(1);
                break;
            case -4:
                ofLog(OF_LOG_FATAL_ERROR) << "serialDataFormat is not a valid TG_STREAM_* type";
                exit(1);
                break;
            default:
                ofLog() << "Connected to ID: " << connectionID << " at: " << deviceName << " ...";
                isConnected = true;
                break;
        }
        
        /*
        if (conResult != 0)
        {
            ofLog(OF_LOG_FATAL_ERROR) << "Error: Connection Failed!";
            exit(1);
        }
        */
        
        int resBlink = TG_EnableBlinkDetection(connectionID, 1);
        if (resBlink != 0)
            ofLog() << "fail to enable blink detection";
    }
    
    void update()
    {
        if (!autoReading) {
            int numPackets = TG_ReadPackets(connectionID, -1);
            if (numPackets > 0)
            {
                signalQuality = TG_GetValue(connectionID, TG_DATA_POOR_SIGNAL);
                
                if (signalQuality == 0.0)
                {
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
                    //////////
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
                    //////////
                    if (TG_GetValueStatus(connectionID, TG_DATA_DELTA) != 0.0)
                    {
                        values[2] = TG_GetValue(connectionID, TG_DATA_DELTA);
                    }
                    //////////
                    if (TG_GetValueStatus(connectionID, TG_DATA_THETA) != 0.0)
                    {
                        values[3] = TG_GetValue(connectionID, TG_DATA_THETA);
                    }
                    //////////
                    if (TG_GetValueStatus(connectionID, TG_DATA_ALPHA1) != 0.0)
                    {
                        values[4] = TG_GetValue(connectionID, TG_DATA_ALPHA1);
                    }
                    //////////
                    if (TG_GetValueStatus(connectionID, TG_DATA_ALPHA2) != 0.0)
                    {
                        values[5] = TG_GetValue(connectionID, TG_DATA_ALPHA2);
                    }
                    //////////
                    if (TG_GetValueStatus(connectionID, TG_DATA_BETA1) != 0.0)
                    {
                        values[6] = TG_GetValue(connectionID, TG_DATA_BETA1);
                    }
                    //////////
                    if (TG_GetValueStatus(connectionID, TG_DATA_BETA2) != 0.0)
                    {
                        values[7] = TG_GetValue(connectionID, TG_DATA_BETA2);
                    }
                    //////////
                    if (TG_GetValueStatus(connectionID, TG_DATA_GAMMA1) != 0.0)
                    {
                        values[8] = TG_GetValue(connectionID, TG_DATA_GAMMA1);
                    }
                    //////////
                    if (TG_GetValueStatus(connectionID, TG_DATA_GAMMA2) != 0.0)
                    {
                        values[9] = TG_GetValue(connectionID, TG_DATA_GAMMA2);
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
                }
            }
        } else {
            cout << "Autoreading is enabled! Use autoUpdate() instead." << endl;
        }
    }
    
    void autoUpdate()
    {
        if (autoReading) {
            // While background auto-reading is enabled, the TG_GetValueStatus() function
            // is pretty much useless. Also, the TG_ReadPackets() function should probably not be called.
            
            signalQuality = TG_GetValue(connectionID, TG_DATA_POOR_SIGNAL);
            
            cout << signalQuality << endl;
            
            if (signalQuality == 0.0)
            {
                
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
                
                
                values[0] = TG_GetValue(connectionID, TG_DATA_ATTENTION);
                values[1] = TG_GetValue(connectionID, TG_DATA_MEDITATION);
                
                for (int i=2; i<10; i++) {
                    values[i] = TG_GetValue(connectionID, i+3);
                }
                
                //Implement some blink shiz?
                
                //cout << values[0] << endl;
            }
        } else {
            cout << "Autoreading is disabled! Use update() instead." << endl;
        }
    }
    
    void startAutoRead() {
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
    
    void stopAutoRead() {
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
    
    void enableBlinkAsClick()
    {
        bEnableBlinkAsClick = true;
    }
    
    void disableBlinkAsClick()
    {
        bEnableBlinkAsClick = false;
    }
    
    float getSignalQuality()
    {
        return signalQuality;
    }
    
    int getID()
    {
        return tgID;
    }
    
    void freeConnection(){
        if (isConnected) {
            TG_FreeConnection(connectionID);
            ofLog() << "disconnecting connection ID: " << connectionID;
            isConnected = false;
        }
    }
    
    ofEvent<float> attentionChangeEvent;
    ofEvent<float> meditationChangeEvent;
    ofEvent<float> blinkChangeEvent;
    
    // blink as click event
    ofEvent<bool> singleClickEvent;
    ofEvent<bool> doubleClickEvent;
    
    float values[10];
    
    
protected:
    // function pointers;
    int (*TG_GetDriverVersion)();
    int (*TG_GetNewConnectionId)();
    int (*TG_Connect)(int, const char *, int, int);
    int (*TG_ReadPackets)(int, int);
    float (*TG_GetValue)(int, int);
    bool (*TG_GetValueStatus)(int, int);
    int (*TG_Disconnect)(int);
    void (*TG_FreeConnection)(int);
    int (*TG_EnableBlinkDetection)(int, int);
    int (*TG_EnableAutoRead)(int,int);
    
    //TG_Connect (int connectionId, const char *serialPortName, int serialBaudrate, int serialDataFormat)
    //TG_EnableAutoRead (int connectionId, int enable)
    
private:
    
    int tgID;
    bool autoReading;
    bool isConnected;
    
    CFURLRef bundleURL;
    CFBundleRef thinkGearBundle;
    
    int connectionID;
    float signalQuality;
    
    bool bEnableBlinkAsClick;
    int prevBlinkTime;
    
};
