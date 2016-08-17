#pragma once

#include "ofMain.h"
#include "ofxCV.h"
#include "ofxFaceTracker.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    
    ofxFaceTracker tracker;
    
};
