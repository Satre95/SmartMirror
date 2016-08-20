#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    stockPlotSetup();
    bool weatherSetup = weatherAPISetup();
    bool surfSetup = surfAPISetup();
    
    fileWatcherSetup();
    
    verdana14.load("verdana.ttf", 14, true, true);
    verdana14.setLineHeight(18.0f);
    verdana14.setLetterSpacing(1.037);
}

//--------------------------------------------------------------
void ofApp::update(){
    if (stockPlotter < stockResult["Elements"][0]["DataSeries"]["close"]["values"].size()){
        stockChart->update(stockData[stockPlotter]);
        stockPlotter++;
    }
    
    fileTimer.update();
    stockTimer.update() ;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    kevinDraw();
    rachitDraw();
    ofSetColor(255);
    // time
    verdana14.drawString(ofGetTimestampString("%A, %B %e %H:%M"), ofGetWindowWidth()-300, 30);
    verdana14.drawString(fileText, 20, 500);
   }

//------------------------------------------------------------------
//MARK: Sid
void ofApp::fileWatcherSetup() {
    ofSaveURLAsync(fileURL, "DisplayText.txt");
    
    fileTimer.setup(fileTimerDelay);
    fileTimer.start(true);
    ofAddListener(fileTimer.TIMER_COMPLETE, this, &ofApp::updateDisplayText);
}

void ofApp::updateDisplayText(int & args) {
    fileTimer.reset();
    ofSaveURLAsync(fileURL, "DisplayText.txt");
    ofBuffer fileBuffer = ofBufferFromFile("DisplayText.txt");
    //ofLogNotice() << fileBuffer.getText();
    fileText = fileBuffer.getText();
}

//--------------------------------------------------------------
//MARK: Kevin

void ofApp::kevinDraw() {
    stockChart->draw(ofGetWindowWidth() - 640, ofGetWindowHeight()-240, 640, 240);
}
void ofApp::stockPlotSetup() {
    stockTimer.setup( 10000 ) ;
    stockTimer.start(true) ;
    
    symbols = {"INTU", "APPLE", "GOOGL", "TSLA"};
    symbolIndex = 0;
    
    ofAddListener( stockTimer.TIMER_COMPLETE , this, &ofApp::stockPlotUpdate ) ;
    
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
    stockChart->setColor( ofColor(255) ); //color of the plot line
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

void ofApp::stockPlotUpdate(int& args) {
        string one = "http://dev.markitondemand.com/MODApis/Api/v2/InteractiveChart/json?parameters=%7B%22Normalized%22%3Afalse%2C%22NumberOfDays%22%3A365%2C%22DataPeriod%22%3A%22Day%22%2C%22Elements%22%3A%5B%7B%22Symbol%22%3A%22";
        string two = "%22%2C%22Type%22%3A%22price%22%2C%22Params%22%3A%5B%22c%22%5D%7D%5D%7D";
        
        string symbol = symbols[symbolIndex++ % symbols.size()];
        
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
    
        stockChart = new ofxHistoryPlot( NULL, "Current " + symbol + " value:", stockResult["Elements"][0]["DataSeries"]["close"]["values"].size(), false); //NULL cos we don't want it to auto-update. confirmed by "true"
        //plot->setRange(50, 150); //hard range, will not adapt to values off-scale
        stockChart->setColor( ofColor(255) ); //color of the plot line
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
    
    stockTimer.reset();
    stockTimer.setup( 15000 ) ;
    stockTimer.start(true) ;

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
        surfIcon.load("images/surfIcon.jpg");
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
    
    int temp = json["main"]["temp"].asInt();
    string description = json["weather"][0]["description"].asString();
    
    string text = description + " with temperature of " + to_string(temp) + "F";
    
    
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

    weatherIcon.draw(0, 5, 100, 100);
    verdana14.drawString(text, 10, 100);
    surfIcon.draw(20, 110, 50, 50);
    verdana14.drawString(surfText, 10, 180);

}
