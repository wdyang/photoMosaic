#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"

#define full_width 640
#define full_height 480
#define icon_width 16
#define icon_height 12
#define mid_width 96
#define mid_height 72


class Mosaic{
public:
    void setup();
    void update();
    void draw();
    
    //    Weidong add code for Mosaic
    int idxImageAtDisplay = 0;
    int idxTargetImage = 0;
    
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
    
    //    For transition, zoom out action
    ofxCvGrayscaleImage midStageMosaic;   //8x8 stitch of full size images
    ofxCvGrayscaleImage finalStageMosaic;  //32x32 stitch of mid size images
    ofxCvGrayscaleImage newMosaic;      //32x32 stitch of icon size images
    int center_x, center_y;         //where the old mosaic fits in the new mosaic
    int mid_corner_x, mid_corner_y;  //midStageMosaic left top corner in the new mosaic index
  
};