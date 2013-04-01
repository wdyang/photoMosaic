#include "mosaicProcess.h"

void MosaicProcess::setup(){
    imageSet.loadImages();
    mosaicSet.init(&imageSet);
    
    //    buildMosaicImage(targetImage);
    displayImage = imageSet.grayImages[idxImageAtDisplay];
    idxTargetImage = idxImageAtDisplay;
    //    z=1;
    
    
}

void MosaicProcess::update(){
    if(z>0){
        displayImage = mosaic.mosaic;
        ofxCvGrayscaleImage img_source = imageSet.grayImages[idxTargetImage];
        
        displayImage.convertToRange(0,  int(255*z));
        img_source.convertToRange(0, int(255*(1-z)));
        displayImage+=img_source;
        z-=0.001;
    }
    
}

void MosaicProcess::draw(){
    imageSet.showIcons(640, 0,  10);
    displayImage.draw(0, 0);
    imageSet.grayImages[idxTargetImage].draw(0,480, mid_width, mid_height);
    
}

void MosaicProcess::advance(){
    idxTargetImage++;
    mosaic.build(idxTargetImage, &imageSet);
    z=1;

}
