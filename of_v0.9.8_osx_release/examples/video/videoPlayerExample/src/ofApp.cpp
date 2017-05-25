#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(kMovieSize-120, kMovieSize);
    
	ofBackground(255,255,255);
	ofSetVerticalSync(true);
	paused = true;

	// Uncomment this to show movies with alpha channels
	movie.setPixelFormat(OF_PIXELS_RGBA);

	movie.load("movies/Bardo_Btson.mp4");
	movie.setLoopState(OF_LOOP_NORMAL);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    
    int baud = 9600;
    serial.setup(0, baud);
    
    nBytesRead = 0;
    startTime = 0;
    memset(bytesReadString, 0, kBufSize);
}

void ofApp::pause(const bool doPause) {
    paused = doPause;
    movie.setPaused(paused);
    
    if (!paused) {
        startTime = ofGetElapsedTimef();
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    movie.update();
    
    if(movie.getIsMovieDone()){
        movie.firstFrame();
        pause(true);
    }
    
    if(serial.available() > 0) {
        unsigned char bytesReturned[kBufSize];
        
        memset(bytesReadString, 0, kBufSize);
        memset(bytesReturned, 0, kBufSize);

        nBytesRead = 0;
        
        int nRead  = 0;  // a temp variable to keep count per read
        while( (nRead = serial.readBytes( bytesReturned, kBufSize)) > 0){
            nBytesRead = nRead;
        };
        
        if (nBytesRead > 0) {
            if (movie.isPaused()) {
                pause(false);
            }
        }
        
        memcpy(bytesReadString, bytesReturned, kBufSize);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetHexColor(0xFFFFFF);
    
    int videoSize = kMovieSize/1.2;

    movie.draw(0, 0, videoSize, videoSize);
    
    ofSetHexColor(0x000000);
    
    int textStart = kMovieSize - 80;
	
    ofDrawBitmapString("<space>=(un)pause, 0-9=jump",
                       20, textStart+0);
    ofDrawBitmapString("frame: " + ofToString(movie.getCurrentFrame()) + "/"+ofToString(movie.getTotalNumFrames()),
                       20, textStart+20);
    ofDrawBitmapString("duration: " + ofToString(movie.getPosition()*movie.getDuration(),2) + "/"+ofToString(movie.getDuration(),2),
                       20, textStart+40);
    ofDrawBitmapString("last coin time " + ofToString(startTime, 3),
                       20, textStart+60);
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    switch(key){
        case ' ':
            pause(!paused);
            break;
        case '0':
            movie.setFrame(movie.getTotalNumFrames() * 0.0);
            break;
        case '1':
            movie.setFrame(movie.getTotalNumFrames() * 0.1);
            break;
        case '2':
            movie.setFrame(movie.getTotalNumFrames() * 0.2);
            break;
        case '3':
            movie.setFrame(movie.getTotalNumFrames() * 0.2);
            break;
        case '4':
            movie.setFrame(movie.getTotalNumFrames() * 0.4);
            break;
        case '5':
            movie.setFrame(movie.getTotalNumFrames() * 0.5);
            break;
        case '6':
            movie.setFrame(movie.getTotalNumFrames() * 0.6);
            break;
        case '7':
            movie.setFrame(movie.getTotalNumFrames() * 0.7);
            break;
        case '8':
            movie.setFrame(movie.getTotalNumFrames() * 0.8);
            break;
        case '9':
            movie.setFrame(movie.getTotalNumFrames() * 0.9);
            break;
    }
}

