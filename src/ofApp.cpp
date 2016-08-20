#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){
    stockPlotSetup();
    setupVideoGrabber();
    bool weatherSetup = weatherAPISetup();
    bool surfSetup = surfAPISetup();
}

//--------------------------------------------------------------
void ofApp::update(){
    stockPlotUpdate();
    updateVideoGrabber();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    kevinDraw();
    rachitDraw();
}

//------------------------------------------------------------------
//MARK: Sid
void ofApp::drawLiveVideo() {
    
}

void ofApp::setupVideoGrabber() {
    grabber.initGrabber(ofGetWidth(), ofGetHeight());

}

void ofApp::updateVideoGrabber() {
    grabber.update();
    if( grabber.isFrameNew()) {
        //Do CV stuff.
    }
}


//--------------------------------------------------------------
//MARK: Kevin

void ofApp::kevinDraw() {
    stockChart->draw(10, 10, 640, 240);
}
void ofApp::stockPlotSetup() {
    
    symbols = {"INTU", "APPLE", "GOOGL", "TSLA"};
    symbolIndex = 0;
    
    string one = "http://dev.markitondemand.com/MODApis/Api/v2/InteractiveChart/json?parameters=%7B%22Normalized%22%3Afalse%2C%22NumberOfDays%22%3A365%2C%22DataPeriod%22%3A%22Day%22%2C%22Elements%22%3A%5B%7B%22Symbol%22%3A%22";
    string two = "%22%2C%22Type%22%3A%22price%22%2C%22Params%22%3A%5B%22c%22%5D%7D%5D%7D";
    
    ofHttpResponse resp = ofLoadURL(one + "INTU" + two);
    
    bool parsingSuccessful = stockResult.parse(resp.data);
    if (parsingSuccessful) {
        
    }
    
    stockData = vector<float>(stockResult["Elements"][0]["DataSeries"]["close"]["values"].size(),0);
    
    float sum = 0;
    
    for (Json::ArrayIndex i = 0; i < stockResult["Elements"][0]["DataSeries"]["close"]["values"].size(); ++i)
    {
        stockData[i] = stockResult["Elements"][0]["DataSeries"]["close"]["values"][i].asFloat();
        sum += stockResult["Elements"][0]["DataSeries"]["close"]["values"][i].asFloat();
    }
    
    float average = sum / stockResult["Elements"][0]["DataSeries"]["close"]["values"].size();
    
    
    stockChart = new ofxHistoryPlot( NULL, "Current stock value:", stockResult["Elements"][0]["DataSeries"]["close"]["values"].size(), false); //NULL cos we don't want it to auto-update. confirmed by "true"
    //plot->setRange(50, 150); //hard range, will not adapt to values off-scale
    stockChart->setColor( ofColor(0,255,0) ); //color of the plot line
    stockChart->setShowNumericalInfo(true);  //show the current value and the scale in the plot
    stockChart->setRespectBorders(true);	   //dont let the plot draw on top of text
    stockChart->setLineWidth(1);				//plot line width
    stockChart->setBackgroundColor(ofColor(0,220)); //custom bg color
    //custom grid setup
    stockChart->setDrawGrid(true);
    stockChart->setGridColor(ofColor(30)); //grid lines color
    stockChart->setGridUnit(14);
    stockChart->setCropToRect(true);
    
    stockPlotter = 0;
}

void ofApp::stockPlotUpdate() {
    
    if (ofGetElapsedTimeMillis() > 10000) {
        string one = "http://dev.markitondemand.com/MODApis/Api/v2/InteractiveChart/json?parameters=%7B%22Normalized%22%3Afalse%2C%22NumberOfDays%22%3A365%2C%22DataPeriod%22%3A%22Day%22%2C%22Elements%22%3A%5B%7B%22Symbol%22%3A%22";
        string two = "%22%2C%22Type%22%3A%22price%22%2C%22Params%22%3A%5B%22c%22%5D%7D%5D%7D";
        
        String symbol = symbols[symbolIndex++ % symbols.size()];
        
        ofHttpResponse resp = ofLoadURL(one + symbol + two);
        
        bool parsingSuccessful = stockResult.parse(resp.data);
        
        stockData = vector<float>(stockResult["Elements"][0]["DataSeries"]["close"]["values"].size(),0);
        
        float sum = 0;
        
        for (Json::ArrayIndex i = 0; i < stockResult["Elements"][0]["DataSeries"]["close"]["values"].size(); ++i)
        {
            stockData[i] = stockResult["Elements"][0]["DataSeries"]["close"]["values"][i].asFloat();
            sum += stockResult["Elements"][0]["DataSeries"]["close"]["values"][i].asFloat();
        }
        
        float average = sum / stockResult["Elements"][0]["DataSeries"]["close"]["values"].size();
        stockPlotter = 0;
        ofResetElapsedTimeCounter();
        
        stockChart = new ofxHistoryPlot( NULL, "Current" + symbol + "value:", stockResult["Elements"][0]["DataSeries"]["close"]["values"].size(), false); //NULL cos we don't want it to auto-update. confirmed by "true"
        //plot->setRange(50, 150); //hard range, will not adapt to values off-scale
        stockChart->setColor( ofColor(0,255,0) ); //color of the plot line
        stockChart->setShowNumericalInfo(true);  //show the current value and the scale in the plot
        stockChart->setRespectBorders(true);	   //dont let the plot draw on top of text
        stockChart->setLineWidth(1);				//plot line width
        stockChart->setBackgroundColor(ofColor(0,220)); //custom bg color
        //custom grid setup
        stockChart->setDrawGrid(true);
        stockChart->setGridColor(ofColor(30)); //grid lines color
        stockChart->setGridUnit(14);
        stockChart->setCropToRect(true);

        
        stockChart->addHorizontalGuide(average, ofColor(255,150,150   )); //add custom reference guides
    }
    
    if (stockPlotter < stockResult["Elements"][0]["DataSeries"]["close"]["values"].size()){
        stockChart->update(stockData[stockPlotter]);
        stockPlotter++;
    }
}



//--------------------------------------------------------------
//MARK: Rachit
bool ofApp::weatherAPISetup()
{
    std::string url = "http://api.openweathermap.org/data/2.5/weather?zip=92123,us&units=imperial&appid=95a59f8290c53cc76ff537e9df438a30";
    bool parsingSuccessful = json.open(url);
    if(parsingSuccessful)
    {
        ofLogNotice("ofApp::setup") << json.getRawString(true);
        string iconId = json["weather"][0]["icon"].asString();
        string url = "http://openweathermap.org/img/w/"+iconId+".png";
        weatherIcon.load(url);
    }
    else {
        ofLogNotice("ofApp::setup") << "Failed to parse JSON.";
    }
    
    
    return parsingSuccessful;
}

bool ofApp::surfAPISetup()
{
    string surfURL = "http://api.spitcast.com/api/spot/forecast/229/";
    bool parsingSuccessful = jsonSurf.open(surfURL);
    if(parsingSuccessful)
    {
        ofLogNotice("ofApp::setup") << jsonSurf.getRawString(true);
    }
    else
    {
        ofLogNotice("ofApp::setup") << "Failed to parse surf JSON.";
    }
    
    return parsingSuccessful;
}

string ofApp::currentTime()
{
    int epoch_time = ofGetUnixTime();
    struct tm * timeinfo;
    
    /* Conversion to time_t as localtime() expects a time_t* */
    time_t epoch_time_as_time_t = epoch_time;
    
    /* Call to localtime() now operates on time_t */
    timeinfo = localtime(&epoch_time_as_time_t);
    char buffer[26];
    strftime(buffer, 26, "%H", timeinfo);
    int hr;
    sscanf(buffer, "%d", &hr);
    string hrString;
    if(hr > 12 && hr != 24)
    {
        hr = hr - 12;
        hrString = to_string(hr) + "PM";
    }
    else if(hr == 12)
    {
        hrString = to_string(hr) + "PM";
    }
    else if(hr == 24)
    {
        hr = hr - 12;
        hrString = to_string(hr) + "AM";
    }
    else
    {
        hrString = to_string(hr)+ "AM";
    }
    return hrString;
}

void ofApp::rachitDraw() {
    string hrString = currentTime();
    string resetTime = "1AM";
    if(!resetTime.compare(hrString))
    {
        weatherAPISetup();
        surfAPISetup();
    }
    
    double temp = json["main"]["temp"].asInt();
    string description = json["weather"][0]["description"].asString();
    
    string text = description + " with temperature of " + to_string(temp) + "F";
    
    ofDrawBitmapString(text, 20, 300);
    weatherIcon.draw(0, 350);
    
    bool found = false;
    string shape;
    int waveSize;
    Json::ArrayIndex i = 0;
    while(!found && i < jsonSurf.size())
    {
        if(!(jsonSurf[i]["hour"].asString()).compare(hrString))
        {
            shape = jsonSurf[i]["shape_full"].asString();
            waveSize = jsonSurf[i]["size_ft"].asInt();
            found = true;
        }
        i++;
    }
    
    string surfText = "Current surf condition is: " + shape + " with wave size of: " + to_string(waveSize) + "Ft";
    ofDrawBitmapString(surfText, 20, 400);

}