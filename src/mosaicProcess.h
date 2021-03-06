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
    bool bZoomOut = true; //by default, we zoom out
    bool AdvanceRandom = true;
    bool bUseEnd = false;
    bool bUseVideo = false;
    bool bUsePresetOnly = false;
    bool bStay = false;
    
    int idxImageAtDisplay = 0;
    int idxTargetImage = 0;
    int idxPre = 0;
    
    int display_w=1024;
    int display_h=768;
    int max_display_w=1024, max_display_h=768;
    
    float hesitation = 1;
    float brightness = 0, contrast = 0;
    
    float z; //0 - 1
    int drawStage; //0 show gray image; 1, transition from mosaic to gary; 2, transition from midstage to mosaic; 3, transition from from to midstage
    
    ofxCvGrayscaleImage mosaicImage;
    ofxCvGrayscaleImage displayImage;
    ofxCvGrayscaleImage showImage;
    
    int mosaicY = full_height/icon_height;
    int mosaicX = full_width/icon_width;
    int mosaicIdx[full_height/icon_height][full_width/icon_width];
    int mosaicAvg[full_height/icon_height][full_width/icon_width];
    
    void advance();
    void transition_mosaic2gray(float delta);
    void transition_mid2mosaic(float delta);
    void transition_from2mid(float delta);
    float speed = 0.01;
    
    float slowStartRatio = 0.0001; //Make initial movement small
};