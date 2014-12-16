//
//  ofxThinkGear.h
//  ofxThinkGear
//
//  Created by Akira Hayasaka on 8/24/12.
//  Copyright (c) 2012 All rights reserved.
//
//
//  Forked repo by Ivaylo Getov April 2014
//  Place ThinkGear.bundle inside "data" folder
//
//  TO DO: implement TG_EnableAutoRead fuction:  From API:
//     "Enabling causes a background thread to be spawned for the
//     connection (only if one was not already previously spawned),
//     which continuously calls TG_ReadPacket( connectionId, -1 ) at 1ms intervals."
//


#pragma once
#include "ofMain.h"

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

const int TG_BAUDRATE = 9600;

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

// Set this to TRUE if you want a connection error to quit your program
const bool errorExit = false;



class ofxThinkGear
{
public:
    
    ofxThinkGear();
    ~ofxThinkGear();
    
    void reset();
    void setup(string deviceName = "/dev/tty.MindWaveMobile-DevA", int _id = 0);
    
    void update();
    void autoUpdate();
    
    // AutoRead Functions ////////////////////
    void startAutoRead();
    void stopAutoRead();
    // End AutoRead Functions ////////////////
    
    void enableBlinkAsClick();
    void disableBlinkAsClick();
    
    float getSignalQuality();
    int getID();
    bool getIsConnected();
    bool getNewInfo();
    
    void freeConnection();
    
    ofEvent<float> attentionChangeEvent;
    ofEvent<float> meditationChangeEvent;
    ofEvent<float> deltaChangeEvent;
    ofEvent<float> thetaChangeEvent;
    ofEvent<float> alpha1ChangeEvent;
    ofEvent<float> alpha2ChangeEvent;
    ofEvent<float> beta1ChangeEvent;
    ofEvent<float> beta2ChangeEvent;
    ofEvent<float> gamma1ChangeEvent;
    ofEvent<float> gamma2ChangeEvent;
    ofEvent<float> blinkChangeEvent;
    
    
    // blink as click event
    ofEvent<bool> singleClickEvent;
    ofEvent<bool> doubleClickEvent;
    
    // an array to hold values read from headset
    float values[10];
    bool ableToConnect;
    
    
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
    
    
private:
    
    int tgID;
    bool autoReading;
    bool isConnected;
    bool newInfo;
    
    CFURLRef bundleURL;
    CFBundleRef thinkGearBundle;
    
    int connectionID;
    float signalQuality;
    
    bool bEnableBlinkAsClick;
    int prevBlinkTime;
    
};
