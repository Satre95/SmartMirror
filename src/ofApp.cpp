#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){
    grabber.initGrabber(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update(){
    grabber.update();
    if( grabber.isFrameNew()) {
        //Do CV stuff.
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
}


void ofApp::drawLiveVideo() {
    
}