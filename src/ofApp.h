#pragma once

#include "ofMain.h"
#include "ofxHistoryPlot.h"
#include "ofxJSON.h"

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
    
    
    //MARK: Rachit
    bool weatherAPISetup();
    bool surfAPISetup();
    string currentTime();
    void rachitDraw();
    ofxJSONElement json;
    ofxJSONElement jsonSurf;
    ofImage weatherIcon;
    

    
};
