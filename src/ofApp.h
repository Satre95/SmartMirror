#pragma once

#include "ofMain.h"
#include "ofxHistoryPlot.h"
#include "ofxJSON.h"
#include "ofxSimpleTimer.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
    
    //MARK: Kevin
    void stockPlotSetup();
    void stockPlotUpdate(int& args);
    ofxHistoryPlot * stockChart;
    ofxJSONElement stockResult;
    std::vector<float> stockData;
    int stockPlotter;
    int symbolIndex;
    vector<string> symbols;
    void kevinDraw();
    ofxSimpleTimer stockTimer ;
    ofTrueTypeFont	verdana14;

    
    //MARK: Sid
    void fileWatcherSetup();
    void updateDisplayText(int & args);
    float fileTimerDelay = 1000;
    ofxSimpleTimer fileTimer;
    ofURLFileLoader fileLoader;
    const string fileURL = "https://dl.dropboxusercontent.com/s/qeobv7jntqjxh14/SmartMirrorText.txt?dl=0";

    
    
    //MARK: Rachit
    bool weatherAPISetup();
    bool surfAPISetup();
    string currentTime();
    void rachitDraw();
    ofxJSONElement json;
    ofxJSONElement jsonSurf;
    ofImage weatherIcon;
    

    
};
