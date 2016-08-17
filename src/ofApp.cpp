#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    stockPlotSetup();

}

//--------------------------------------------------------------
void ofApp::update(){
    stockPlotUpdate();

}

//--------------------------------------------------------------
void ofApp::draw(){
    stockChart->draw(10, 10, 640, 240);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::stockPlotSetup() {
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
    stockChart->addHorizontalGuide(average, ofColor(255,150,150   )); //add custom reference guides
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
    if (stockPlotter < stockResult["Elements"][0]["DataSeries"]["close"]["values"].size()){
        stockChart->update(stockData[stockPlotter]);
        stockPlotter++;
    }
}