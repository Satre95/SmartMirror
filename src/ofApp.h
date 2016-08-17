#pragma once

#include "ofMain.h"
#include "ofxHistoryPlot.h"
#include "ofxJSON.h"
#include "ofxCV.h"
#include "ofxFaceTracker.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
    
        void stockPlotSetup();
    void stockPlotUpdate();
    void setupVideoGrabber();
    void updateVideoGrabber();
    void drawLiveVideo();
    
        ofxHistoryPlot * stockChart;
        ofxJSONElement stockResult;
        std::vector<float> stockData;
        int stockPlotter;
    ofxFaceTracker tracker;
    ofVideoGrabber grabber;
    
};
