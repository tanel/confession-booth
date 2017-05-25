#pragma once

#include "ofMain.h"

#define kBufSize 100
#define kMovieSize 700

class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
        void pause(const bool doPause);

		ofVideoPlayer 		movie;
		bool                paused;
        float		startTime;
    
        bool		bSendSerialMessage;			// a flag for sending serial
        char		bytesRead[kBufSize];				// data from serial, we will be trying to read 3
        char		bytesReadString[kBufSize];			// a string needs a null terminator, so we need 3 + 1 bytes
        int			nBytesRead;					// how much did we read?
    
        ofSerial	serial;
    
};

