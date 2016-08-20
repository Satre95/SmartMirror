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
    
    //MARK: Kevin
    void stockPlotSetup();
    void stockPlotUpdate();
    ofxHistoryPlot * stockChart;
    ofxJSONElement stockResult;
    std::vector<float> stockData;
    int stockPlotter;
    int symbolIndex;
    vector<string> symbols;
    void kevinDraw();

    
    //MARK: Sid
    void setupVideoGrabber();
    void updateVideoGrabber();
    void drawLiveVideo();
    ofxFaceTracker tracker;
    ofVideoGrabber grabber;
    
    //MARK: Rachit
    bool weatherAPISetup();
    bool surfAPISetup();
    string currentTime();
    void rachitDraw();
    ofxJSONElement json;
    ofxJSONElement jsonSurf;
    ofImage weatherIcon;
    

    
};
