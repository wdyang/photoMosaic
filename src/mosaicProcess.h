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
    bool AdvanceRandom = true;
    bool UseEnd = false;
    
    int idxImageAtDisplay = 0;
    int idxTargetImage = 0;
    
    float z; //0 - 1
    int drawStage; //0 show gray image; 1, transition from mosaic to gary; 2, transition from midstage to mosaic; 3, transition from from to midstage
    
    ofxCvGrayscaleImage mosaicImage;
    ofxCvGrayscaleImage displayImage;
    
    int mosaicY = full_height/icon_height;
    int mosaicX = full_width/icon_width;
    int mosaicIdx[full_height/icon_height][full_width/icon_width];
    int mosaicAvg[full_height/icon_height][full_width/icon_width];
    
    void advance();
    void transition_mosaic2gray(float delta);
    void transition_mid2mosaic(float delta);
    void transition_from2mid(float delta);
    float speed = 0.01;
};