ofxThinkGear
===================================

Description
-----------

openFrameworks addon for NeuroSky ThinkGear bundle.
ofxThinkGear utilizes [NeuroSky ThinkGear](http://www.neurosky.com/Business/ThinkGearChipsets.aspx) via BlueTooth connection to get brainwave (attention and meditation).

Build requirement
-----------

You need to download ThinkGear.bundle from [here](http://store.neurosky.com/products/developer-tools-2-1) and put it in your project's "data" directory.

Usage
-----

See the example and `src/ofxThinkGear.h`.

All of the available data points triggger ofEvents when they are updated. You only
need to declare listeners for the ones you need (usually just attention and meditation).

Even though you are calling thinkGear.update() every time through testApp::update(),
you will only get an event when the headset has new data, which is about once
a second. 

This also ensures you are not reassigning old data to a variable. In this
example, the graph will only grow when the data is updated.

Make sure you call thinkGear.freeConnection() in testApp::exit() to properly 
close the connection to the headset.


