// Maintains a collection of image for producing mosaic
#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "time.h"

class ImageSet{
    ofDirectory dir;

public:
    vector<ofxCvGrayscaleImage> grayImages;
    vector<ofxCvGrayscaleImage> grayMids;
    vector<ofxCvGrayscaleImage> grayIcons;
    vector<float> imageMeans;

    int numImages, numPresetImages;
    
    void addImage(ofxCvGrayscaleImage imageIn);
    void loadImages();
    void setImageFromFile(int i, string fname);
    void showIcons(int x, int y, int numCol);
    int getMatchingIcon(int val);
    void saveImage(ofxCvGrayscaleImage image);
    
    ofxCvGrayscaleImage 	tempGrayImage; //place holder for testApp to save grayImage here
};