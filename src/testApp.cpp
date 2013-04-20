#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	#ifdef _USE_LIVE_VIDEO
//        int numDevices = vidGrabber.listDevices();
    
        vidGrabber.setVerbose(true);
    vidGrabber.setDeviceID(0);
    if(vidGrabber.initGrabber(cam_w,cam_h)){
//        vidGrabber.setDeviceID(0);
//        vidGrabber.initGrabber(cam_w,cam_h);
    };
	#else
        vidPlayer.loadMovie("fingers.mov");
        vidPlayer.play();
	#endif

    colorImg.allocate(cam_w,cam_h);
	grayImage.allocate(cam_w,cam_h);
	grayBg.allocate(cam_w,cam_h);
	grayDiff.allocate(cam_w,cam_h);

	bLearnBakground = true;
	threshold = 80;
//    mosaicProcess = new MosaicProcess();
    mosaicProcess.setup();
    imageSet = &(mosaicProcess.imageSet);
    
//    Setup TouchOSC
    ipadReceiver.setup(PORT_FROM_IPAD);
//    ipadSender.setup(HOST_IPAD, PORT_TO_IPAD);
    
    
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(100,100,100);

    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.update();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.update();
        bNewFrame = vidPlayer.isFrameNew();
	#endif

	if (bNewFrame){

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels(), cam_w,cam_h);
	    #else
            colorImg.setFromPixels(vidPlayer.getPixels(), cam_w,cam_h);
        #endif

        grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (cam_w*cam_h)/3, 10, true);	// find holes
	}
    
    if(bUpdateMosaic) mosaicProcess.update();
    
    while(ipadReceiver.hasWaitingMessages()){
        parseIpadOSCMessage();
    }
}

//--------------------------------------------------------------
void testApp::draw(){

	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
//	colorImg.draw(20,20);
	grayImage.draw(640,0);
//	grayBg.draw(20,280);
//	grayDiff.draw(360,280);

	// then draw the contours:

//	ofFill();
//	ofSetHexColor(0x333333);
//	ofRect(360,540,320,240);
//	ofSetHexColor(0xffffff);

	// we could draw the whole contour finder
	//contourFinder.draw(360,540);

	// or, instead we can draw each blob individually from the blobs vector,
	// this is how to get access to them:
//    for (int i = 0; i < contourFinder.nBlobs; i++){
//        contourFinder.blobs[i].draw(360,540);
//		
//		// draw over the centroid if the blob is a hole
//		ofSetColor(255);
//		if(contourFinder.blobs[i].hole){
//			ofDrawBitmapString("hole",
//				contourFinder.blobs[i].boundingRect.getCenter().x + 360,
//				contourFinder.blobs[i].boundingRect.getCenter().y + 540);
//		}
//    }
//
//	// finally, a report:
//	ofSetHexColor(0xffffff);
//	stringstream reportStr;
//	reportStr << "bg subtraction and blob detection" << endl
//			  << "press ' ' to capture bg" << endl
//			  << "threshold " << threshold << " (press: +/-)" << endl
//			  << "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate();
//	ofDrawBitmapString(reportStr.str(), 20, 600);
    mosaicProcess.draw();

}


//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key){
		case ' ':
			bUpdateMosaic = !bUpdateMosaic;
			break;
		case 'l':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
		case 'z':
            mosaicProcess.bZoomOut = ! mosaicProcess.bZoomOut;
			break;
        case 'n':
            imageSet->addImage(grayImage);
            mosaicProcess.UseEnd = true;
            imageSet->saveImage(grayImage);
            break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
