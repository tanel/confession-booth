#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowShape(kMovieSize, kMovieSize);
    
	ofBackground(255,255,255);
	ofSetVerticalSync(true);
	paused = true;

	// Uncomment this to show movies with alpha channels
	movie.setPixelFormat(OF_PIXELS_RGBA);

	movie.load("movies/videoplayback.mp4");
	movie.setLoopState(OF_LOOP_NORMAL);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    serial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    
    // this should be set to whatever com port your serial device is connected to.
    // (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
    // arduino users check in arduino app....
    int baud = 9600;
    serial.setup(0, baud); //open the first device
    //serial.setup("COM4", baud); // windows example
    //serial.setup("/dev/tty.usbserial-A4001JEC", baud); // mac osx example
    //serial.setup("/dev/ttyUSB0", baud); //linux example
    
    nBytesRead = 0;
    readTime = 0;
    memset(bytesReadString, 0, kBufSize);
}

//--------------------------------------------------------------
void ofApp::update(){
    movie.update();
    
    if(movie.getIsMovieDone()){
        movie.firstFrame();
        paused = true;
        movie.setPaused(paused);
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
                paused = false;
                movie.play();
            }
        }
        
        memcpy(bytesReadString, bytesReturned, kBufSize);
        
        readTime = ofGetElapsedTimef();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetHexColor(0xFFFFFF);

    movie.draw(0,0,kMovieSize,kMovieSize);
    
    ofSetHexColor(0x000000);
	
    ofDrawBitmapString("p=(un)pause, 0=rewind",20,320);
    ofDrawBitmapString("frame: " + ofToString(movie.getCurrentFrame()) + "/"+ofToString(movie.getTotalNumFrames()),20,340);
    ofDrawBitmapString("duration: " + ofToString(movie.getPosition()*movie.getDuration(),2) + "/"+ofToString(movie.getDuration(),2),20,360);
    ofDrawBitmapString("last coin time " + ofToString(readTime, 3), 20, 380);
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    switch(key){
        case 'p':
            paused = !paused;
            movie.setPaused(paused);
        break;
        case '0':
            movie.firstFrame();
            paused = true;
            movie.setPaused(paused);
        break;
    }
}

