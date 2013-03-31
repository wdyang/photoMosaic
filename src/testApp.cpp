#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	#ifdef _USE_LIVE_VIDEO
        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(320,240);
	#else
        vidPlayer.loadMovie("fingers.mov");
        vidPlayer.play();
	#endif

    colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	grayBg.allocate(320,240);
	grayDiff.allocate(320,240);

	bLearnBakground = true;
	threshold = 80;
    
    loadImages();

    buildMosaicImage(targetImage);
    displayImage = mosaicImage;
    z=1;
}
void testApp::loadImages(){
    dir.listDir("images/");
    dir.sort();
    numImages = dir.size();
    cout<<"loaded "<<numImages<<" images.";
    if(dir.size()){
        grayImages.assign(numImages, ofxCvGrayscaleImage());
        grayIcons.assign(numImages, ofxCvGrayscaleImage());
        imageMeans.assign(numImages, float());
        
        for(int i=0; i<numImages; i++){
            cout<<"reading image: "<<dir.getPath(i);
            setImageFromFile(i,dir.getPath(i));
        }
    }
}

void testApp::setImageFromFile(int i, string fname){
    ofImage tempImage;
    tempImage.loadImage(fname);
    
    float full_ratio = (float)full_width/ (float)full_height;
    cout<<"full_ratio is "<<full_ratio;

    int h = tempImage.getHeight();
    int w = tempImage.getWidth();
    float ratio = (float)w / (float) h;
    if(ratio >= full_ratio){   //image is wider than needed, crop width from center
        int new_width = (int)(ratio * full_height);
        tempImage.resize(new_width, full_height);
        int x0 = (new_width - full_width)/2;
        tempImage.crop(x0, 0, full_width, full_height);
    }else{   //image is higher than needed, crop height from center
        int new_height = (int) full_width / ratio;
        tempImage.resize(full_width, new_height);
        int y0 = (new_height - full_height)/2;
        tempImage.crop(0, y0, full_width, full_height);
    }
    
    ofxCvColorImage cvColor;
    cvColor.setFromPixels(tempImage.getPixels(), full_width, full_height);
    grayImages[i] = cvColor;
    grayIcons[i]=grayImages[i];
    grayIcons[i].resize(icon_width, icon_height);
    
    CvScalar avg = cvAvg(grayIcons[i].getCvImage());
    imageMeans[i] = avg.val[0];
//    cout<<"image "<<i<<" has average of "<<imageMeans[i];
    
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
            colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
	    #else
            colorImg.setFromPixels(vidPlayer.getPixels(), 320,240);
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
		contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, true);	// find holes
	}
    
    if(z>0){
        displayImage = mosaicImage;
        ofxCvGrayscaleImage img_source = grayImages[targetImage];
        
        displayImage.convertToRange(0,  int(255*z));
        img_source.convertToRange(0, int(255*(1-z)));
        displayImage+=img_source;
        z-=0.001;
    }

}

//--------------------------------------------------------------
void testApp::draw(){

	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
	colorImg.draw(20,20);
	grayImage.draw(360,20);
	grayBg.draw(20,280);
	grayDiff.draw(360,280);

	// then draw the contours:

	ofFill();
	ofSetHexColor(0x333333);
	ofRect(360,540,320,240);
	ofSetHexColor(0xffffff);

	// we could draw the whole contour finder
	//contourFinder.draw(360,540);

	// or, instead we can draw each blob individually from the blobs vector,
	// this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(360,540);
		
		// draw over the centroid if the blob is a hole
		ofSetColor(255);
		if(contourFinder.blobs[i].hole){
			ofDrawBitmapString("hole",
				contourFinder.blobs[i].boundingRect.getCenter().x + 360,
				contourFinder.blobs[i].boundingRect.getCenter().y + 540);
		}
    }

	// finally, a report:
	ofSetHexColor(0xffffff);
	stringstream reportStr;
	reportStr << "bg subtraction and blob detection" << endl
			  << "press ' ' to capture bg" << endl
			  << "threshold " << threshold << " (press: +/-)" << endl
			  << "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate();
	ofDrawBitmapString(reportStr.str(), 20, 600);

    showIcons(640, 0,  10);
    grayImages[targetImage].draw(0,0);
    displayImage.draw(0, 480);
}

void testApp::showIcons(int x, int y, int numCol){
    int row = 0, col = 0;
    for(int i=0; i<numImages; i++){
        grayIcons[i].draw(x+col*icon_width, y+row*icon_height);
        col++;
        if(col>=numCol){
            row++;
            col=0;
        }
    }
    
}

void testApp::buildMosaicImage(int targetImageIdx){
    //first shrink image down
    ofxCvGrayscaleImage tempImage = grayImages[targetImageIdx];
    tempImage.resize(mosaicX, mosaicY);
    IplImage * cvImage =   tempImage.getCvImage();
    mosaicImage = grayImages[targetImage];
    
    cout<< "image widthstep is "<<tempImage.getCvImage()->widthStep;
    unsigned char* pixels = tempImage.getPixels();
    for (int i=0; i<mosaicY; i++){
        unsigned char* ptr = (unsigned char*)(cvImage->imageData + i * cvImage->widthStep);
        for (int j=0; j<mosaicX; j++){
            cout<<(int) ptr[j]<<"(";
            mosaicAvg[i][j]=(int) ptr[j];
            mosaicIdx[i][j] = getMatchingIcon((int) ptr[j]);
            cout<<mosaicIdx[i][j]<<")";
            mosaicImage.setROI(j*icon_width, i*icon_height, icon_width, icon_height);
            mosaicImage.setRoiFromPixels(grayIcons[mosaicIdx[i][j]].getPixels(), icon_width, icon_height);
        }
        cout<<endl;
    }
    //clear ROI
    mosaicImage.setROI(0,0,full_width,full_height);

}

int testApp::getMatchingIcon(int val){
    float diff = 1000000;
    int idx;
    for(int i=0; i<numImages; i++){
        if(abs(val - imageMeans[i])<diff){
            diff = abs(val-imageMeans[i]);
            idx =i;
        }
    }
    return idx;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key){
		case ' ':
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
        case 'n':
            targetImage++;
            buildMosaicImage(targetImage);
            z=1;
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
