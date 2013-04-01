#pragma once

#include "ofMain.h"
#include "imageSet.h"
#include "ofxOpenCv.h"
#include "mosaicSet.h"


class MosaicProcess{
public:
    void setup();
    void update();
    void draw();
    
    Mosaic mosaic;
    ImageSet imageSet;
    MosaicSet mosaicSet;
    
    int idxImageAtDisplay = 0;
    int idxTargetImage = 0;
    
    float z;
    
    ofxCvGrayscaleImage mosaicImage;
    ofxCvGrayscaleImage displayImage;
    
    int mosaicY = full_height/icon_height;
    int mosaicX = full_width/icon_width;
    int mosaicIdx[full_height/icon_height][full_width/icon_width];
    int mosaicAvg[full_height/icon_height][full_width/icon_width];
    
    void advance();
    
    //    For transition, zoom out action
//    ofxCvGrayscaleImage midStageMosaic;   //8x8 stitch of full size images
//    ofxCvGrayscaleImage finalStageMosaic;  //32x32 stitch of mid size images
//    ofxCvGrayscaleImage newMosaic;      //32x32 stitch of icon size images
//    int center_x, center_y;         //where the old mosaic fits in the new mosaic
//    int mid_corner_x, mid_corner_y;  //midStageMosaic left top corner in the new mosaic index
    
};