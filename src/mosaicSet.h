#pragma once

#include "ofMain.h"
#include "imageSet.h"

#include "ofxOpenCv.h"

#define full_width 640
#define full_height 480
#define icon_width 16
#define icon_height 12
#define mid_width 128
#define mid_height 96

class Mosaic{
public:
    ofxCvGrayscaleImage mosaic;
    int idxImage;
    ImageSet* imageSet;
    
    int mosaicYSteps = full_height/icon_height;
    int mosaicXSteps = full_width/icon_width;
    int mosaicIdx[full_height/icon_height][full_width/icon_width];
    int mosaicAvg[full_height/icon_height][full_width/icon_width];

    void build(int _idxImage, ImageSet* _imageSet);

};


class MosaicSet{
    ImageSet *imageSet;
    
    int mosaicXSteps=full_width/icon_width;
    int mosaicYSteps=full_height/icon_height;
    
    int idxFromImage;
    int idxToImage;
    
    
    
    ofxCvGrayscaleImage mosaicStage1; //5x5 full_size with fromImage in the center
    ofxCvGrayscaleImage mosaicStage2; //40x40 mid_size with mosaicStage1 in the center
    ofxCvGrayscaleImage mosaic; //40*40 icon_size

public:
    void init(ImageSet* _imageSet);
    
    void buildSet(int _from, int _to);
    
};