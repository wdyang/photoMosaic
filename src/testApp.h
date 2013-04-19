#pragma once

#include "ofMain.h"
#include "imageSet.h"

#include "ofxOpenCv.h"

#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
								// otherwise, we'll use a movie file
#include "mosaicProcess.h"


//OSC
#include "ofxOsc.h"
//For comminicating with TouchOSC on ipad
#define HOST_IPAD "192.168.2.2" //ipad ip
//#define HOST_IPAD "10.0.1.5" //ipad ip
#define PORT_FROM_IPAD 8000
#define PORT_TO_IPAD 9000


class testApp : public ofBaseApp{

public:
    int cam_w = 640;
    int cam_h = 480;
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);		

    #ifdef _USE_LIVE_VIDEO
      ofVideoGrabber 		vidGrabber;
    #else
      ofVideoPlayer 		vidPlayer;
    #endif

    ofxCvColorImage			colorImg;

    ofxCvGrayscaleImage 	grayImage;
    ofxCvGrayscaleImage 	grayBg;
    ofxCvGrayscaleImage 	grayDiff;

    ofxCvContourFinder 	contourFinder;

    int 				threshold;
    bool				bLearnBakground;

    MosaicProcess mosaicProcess;
    ImageSet *imageSet;
    bool    bUpdateMosaic=true;
    
    
    
//     OSC control for touchOSC
    ofxOscReceiver ipadReceiver;
    ofxOscSender    ipadSender;
    string ipadIP;
    bool    bSenderLive;
    void parseIpadOSCMessage();
    void oscSendInitConfig();
    void oscSendInt(const string &address, int msg);
    void oscSendFloat(const string &address, float msg);
    void oscSendFormatedFloat(const string &address, float msg, int precision); //precision is the number of decimal points
    void oscSendString(const string &address, const string &msg);

};

