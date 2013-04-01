#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"

class ImageSet{
    ofDirectory dir;

public:
    vector<ofxCvGrayscaleImage> grayImages;
    vector<ofxCvGrayscaleImage> grayMids;
    vector<ofxCvGrayscaleImage> grayIcons;
    vector<float> imageMeans;

    int numImages;
    
    void loadImages();
    void setImageFromFile(int i, string fname);
    void showIcons(int x, int y, int numCol);
    int getMatchingIcon(int val);

};