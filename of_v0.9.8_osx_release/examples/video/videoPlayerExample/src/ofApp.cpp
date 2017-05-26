#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(kMovieSize-120, kMovieSize);
    
	ofBackground(255,255,255);
	ofSetVerticalSync(true);
	paused = true;

	// Show movies with alpha channels
	movie.setPixelFormat(OF_PIXELS_RGBA);

    // Load video
	movie.load("movies/Bardo_Btson.mp4");
	movie.setLoopState(OF_LOOP_NORMAL);
    
    ofSetLogLevel(OF_LOG_VERBOSE);

    // Initialize serial input
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    
    int baud = 9600;
    serial.setup(0, baud);
    
    nBytesRead = 0;
    memset(bytesReadString, 0, kBufSize);
    
    // Initialize coin input
    startTime = 0;
    
    // Initialize audio input
    //if you want to set a different device id
    //soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
    soundStream.printDeviceList();

    int bufferSize = 256;
    
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);
    
    bufferCounter	= 0;
    drawCounter		= 0;
    smoothedVol     = 0.0;
    scaledVol		= 0.0;
    
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
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
    
    int displayWidth = ofGetWindowWidth();
    int displayHeight = ofGetWindowHeight() - 120;
    
    movie.draw(0, 0, displayWidth, displayHeight);
    
    int alphaPercentage = 10;

    int alpha = alphaPercentage * 255 / 100;
    
    ofEnableAlphaBlending();
    ofSetColor(0, 0, 0, alpha);
    ofDrawRectangle(1, 1, displayWidth, displayHeight);
    ofDisableAlphaBlending();
    
    
    ofSetHexColor(0x000000);
    
    int textStart = ofGetWindowHeight() - 80;
	
    ofDrawBitmapString("<space>=(un)pause, 0-9=jump, q=exit",
                       20, textStart+0);
    ofDrawBitmapString("frame: " + ofToString(movie.getCurrentFrame()) + "/"+ofToString(movie.getTotalNumFrames()),
                       20, textStart+20);
    ofDrawBitmapString("duration: " + ofToString(movie.getPosition()*movie.getDuration(),2) + "/"+ofToString(movie.getDuration(),2),
                       20, textStart+40);
    ofDrawBitmapString("last coin time " + ofToString(startTime, 3),
                       20, textStart+60);
}

//--------------------------------------------------------------
void ofApp::windowResized(int width, int height) {
    
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    switch(key){
        case ' ':
            pause(!paused);
            break;
        case 'q':
            ofExit();
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

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    float curVol = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0;
    
    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    for (int i = 0; i < bufferSize; i++){
        left[i]		= input[i*2]*0.5;
        right[i]	= input[i*2+1]*0.5;
        
        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted+=2;
    }
    
    //this is how we get the mean of rms :)
    curVol /= (float)numCounted;
    
    // this is how we get the root of rms :)
    curVol = sqrt( curVol );
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    
    bufferCounter++;
}
