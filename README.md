ofxThinkGear
===================================

Description
-----------

openFrameworks addon for NeuroSky ThinkGear bundle for OSX.   

Works with [NeuroSky ThinkGear](http://www.neurosky.com/Business/ThinkGearChipsets.aspx) via BlueTooth. Tested with MindWave Mobile on OF 0.8.0 to 0.8.4


Build requirement
-----------

You need to download ThinkGear.bundle from [here](http://store.neurosky.com/products/developer-tools) and put it in the `libs` directory.

If you want to make your app portable, the ThinkGear.bundle needs to travel with your app. See the comments in `src/ofxThinkGear.cpp` for details on how to make that happen.

Usage
-----

See the example for basic usage.

All of the available data points triggger ofEvents when they are updated. You only need to declare listeners for the ones you need (usually just attention and meditation).
     
Even though you are calling `thinkGear.update()` every time through `ofApp::update()`, you will only get an event when the headset has new data, which is about once a second.
     
If you don't want to use events, you can also poll the most recent values on demand using getValue(int v) using the following values for v:

     0 - Attention   |   5 - Alpha 2
     1 - Meditation  |   6 - Beta 1
     2 - Delta       |   7 - Beta 2
     3 - Theta       |   8 - Gamma 1
     4 - Alpha 1     |   9 - Gamma 2
     
Make sure you call `thinkGear.freeConnection()` in `ofApp::exit(`) to properly close the connection to the headset.


To Do:
-----

* Implement TG_EnableAutoRead fuction:  From API:
  
  _"Enabling causes a background thread to be spawned for the
connection (only if one was not already previously spawned),
which continuously calls TG_ReadPacket( connectionId, -1 ) at 1ms intervals."_

* The ThinkGear Communication Driver (aka, using the ThinkGear.bundle) is deprecated as of Version 2.5 of the Mindwave Developer Tools. Need to change this to use the Thinkgear SDK (the ThinkGear.framework) when OF updates to 64-bit.

* Make a Windows (.NET) version


Thanks
-----

This is a fork of Akira Hayasaka's [ofxThinkGear](https://github.com/Akira-Hayasaka/ofxThinkGear). All the hard stuff was already done there.
