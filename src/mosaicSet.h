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
    ofxCvGrayscaleImage mosaic;  //this is the stitched together image
    int idxImage;   //the idx of the source image

    ImageSet* imageSet;
    
    int mosaicYSteps = full_height/icon_height;
    int mosaicXSteps = full_width/icon_width;
    int mosaicIdx[full_height/icon_height][full_width/icon_width]; //the original image corresponding to the icon
    int mosaicAvg[full_height/icon_height][full_width/icon_width]; //[Y][X]

    void build(int _idxImage, ImageSet* _imageSet);
};

//fromImage is a part of toImage, need a mid stage mosaic to transform to
class MosaicSet{
    ImageSet *imageSet;
    
    int mosaicXSteps=full_width/icon_width;
    int mosaicYSteps=full_height/icon_height;

public:
    int idxFromImage;
    int idxToImage;
    
    int fromInTo_Y;  //this is where from image located in the to image mosaic
    int fromInTo_X;
    
    ofxCvGrayscaleImage mosaicStageFullRes; //5x5 full_size with fromImage in the center
    ofxCvGrayscaleImage mosaicStageMidRes; //40x40 mid_size with mosaicStage1 in the center
    Mosaic mosaic; //40*40 icon_size final mosaic

    void init(ImageSet* _imageSet);
    
    void buildSet(int _from, int _to);
    
};