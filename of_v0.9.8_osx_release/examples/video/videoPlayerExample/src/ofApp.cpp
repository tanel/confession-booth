#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255,255,255);
	ofSetVerticalSync(true);
	paused = true;

	// Uncomment this to show movies with alpha channels
	fingerMovie.setPixelFormat(OF_PIXELS_RGBA);

	fingerMovie.load("movies/videoplayback.mp4");
	fingerMovie.setLoopState(OF_LOOP_NORMAL);
	fingerMovie.play();
}

//--------------------------------------------------------------
void ofApp::update(){
    fingerMovie.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetHexColor(0xFFFFFF);

    fingerMovie.draw(20,20);
    ofSetHexColor(0x000000);
    ofPixels & pixels = fingerMovie.getPixels();

    int vidWidth = pixels.getWidth();
    int vidHeight = pixels.getHeight();
    int nChannels = pixels.getNumChannels();
    
    // let's move through the "RGB(A)" char array
    // using the red pixel to control the size of a circle.
    for (int i = 4; i < vidWidth; i+=8){
        for (int j = 4; j < vidHeight; j+=8){
            unsigned char r = pixels[(j * 320 + i)*nChannels];
            float val = 1 - ((float)r / 255.0f);
			ofDrawCircle(400 + i,20+j,10*val);
        }
    }


    ofSetHexColor(0x000000);
	ofDrawBitmapString("press p to (un)pause",20,320);
    ofSetHexColor(0x000000);

    ofDrawBitmapString("frame: " + ofToString(fingerMovie.getCurrentFrame()) + "/"+ofToString(fingerMovie.getTotalNumFrames()),20,380);
    ofDrawBitmapString("duration: " + ofToString(fingerMovie.getPosition()*fingerMovie.getDuration(),2) + "/"+ofToString(fingerMovie.getDuration(),2),20,400);
    ofDrawBitmapString("speed: " + ofToString(fingerMovie.getSpeed(),2),20,420);

    if(fingerMovie.getIsMovieDone()){
        ofSetHexColor(0xFF0000);
        ofDrawBitmapString("end of movie",20,440);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    switch(key){
        case 'p':
            paused = !paused;
            fingerMovie.setPaused(paused);
        break;
        case '0':
            fingerMovie.firstFrame();
        break;
    }
}

