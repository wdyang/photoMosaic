#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"

#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
								// otherwise, we'll use a movie file

#define full_width 640
#define full_height 480
#define icon_width 16
#define icon_height 12


class testApp : public ofBaseApp{

	public:
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


    
//    Weidong add code for Mosaic
        int targetImage = 0;
    
//        const int full_width=320;
//        int full_height = 240;
//        const int icon_width = 32;
//        int icon_height = 24;
        int numImages;
        float z;
    
        ofxCvGrayscaleImage mosaicImage;
        ofxCvGrayscaleImage displayImage;
    
        int mosaicY = full_height/icon_height;
        int mosaicX = full_width/icon_width;
        int mosaicIdx[full_height/icon_height][full_width/icon_width];
        int mosaicAvg[full_height/icon_height][full_width/icon_width];
    
        ofDirectory dir;
    
        vector<ofxCvGrayscaleImage> grayImages;
        vector<ofxCvGrayscaleImage> grayIcons;
        vector<float> imageMeans;

        void loadImages();
        void setImageFromFile(int i, string fname);
        void showIcons(int x, int y, int col);
        void buildMosaicImage(int targetImageIdx);
        int getMatchingIcon(int val);
};

