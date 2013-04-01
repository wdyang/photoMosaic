#include "mosaicProcess.h"

void MosaicProcess::setup(){
    imageSet.loadImages();
    mosaicSet.init(&imageSet);
    
    //    buildMosaicImage(targetImage);
    displayImage = imageSet.grayImages[idxImageAtDisplay];
    idxTargetImage = idxImageAtDisplay;

    z=0;
    drawStage = 0;
    
    
}

void MosaicProcess::update(){
    if(drawStage==0){
        if(z>0){
            z-=0.01;
        }else{
            advance();
        }
    }
    if(drawStage==1){
        transition_mosaic2gray();
    }else if (drawStage==2){
        transition_mid2mosaic();
    }else if(drawStage==3){
        transition_from2mid();
    }
}

void MosaicProcess::transition_mosaic2gray(){
//    drawStage is 1
    if(z>0){
        displayImage = mosaic.mosaic;
        ofxCvGrayscaleImage img_source = imageSet.grayImages[idxTargetImage];
        
        displayImage.convertToRange(0,  int(255*z));
        img_source.convertToRange(0, int(255*(1-z)));
        displayImage+=img_source;
        z-=0.002;
    }else{
        cout<<"mosaic 2 gray done, showing gray image now"<<endl;
        drawStage--;
        z=1;
    }
    
}

void MosaicProcess::transition_mid2mosaic(){
//    drawStage is 2
    //    start, ROI set at
    int x0=(mosaicSet.fromInTo_X-2)*mid_width;
    int y0=(mosaicSet.fromInTo_Y-2)*mid_height;
    int w0 = full_width;
    int h0 = full_height;
    int x1=0;
    int y1=0;
    int w1 = 8*full_width;
    int h1 = 8*full_height;
    int x,y,w, h;
    if(z>0){
        float z1=sqrt(z);
        x=int(z1*x0 + (1-z1)*x1);
        y=int(z1*y0 + (1-z1)*y1);
        w=int(z1*w0 + (1-z1)*w1);
        h=int(z1*h0 + (1-z1)*h1);
        
        mosaicSet.mosaicStageMidRes.setROI(x, y, w, h);
        displayImage.setFromPixels(mosaicSet.mosaicStageMidRes.getRoiPixels(), w, h);
        displayImage.resize(full_width, full_height);
        z-=0.001;
    }else{
        cout<<"mid 2 mosaic done, transition to mosaic 2 gray"<<endl;
        drawStage--;
        z=1;
    }
}

void MosaicProcess::transition_from2mid(){
//    drawStage is 3
    //    start, ROI set at
    int x0=2*full_width;
    int y0=2*full_height;
    int w0 = full_width;
    int h0 = full_height;
    int x1=0;
    int y1=0;
    int w1 = 5*full_width;
    int h1 = 5*full_height;
    int x,y,w, h;
    if(z>0){
        float z1= sqrt(z);
        x=int(z1*x0 + (1-z1)*x1);
        y=int(z1*y0 + (1-z1)*y1);
        w=int(z1*w0 + (1-z1)*w1);
        h=int(z1*h0 + (1-z1)*h1);
        
        mosaicSet.mosaicStageFullRes.setROI(x,y,w,h);
        displayImage.setFromPixels(mosaicSet.mosaicStageFullRes.getRoiPixels(), w, h);
        displayImage.resize(full_width, full_height);

        z-=0.002;
    }else{
        drawStage--;
        z=1;
    }

}

void MosaicProcess::draw(){
    imageSet.showIcons(640, 0,  10);
    displayImage.draw(0, 0);
    imageSet.grayImages[idxTargetImage].draw(0,480, mid_width, mid_height);
    
}

void MosaicProcess::advance(){
    int idxPre = idxTargetImage;
    if(UseEnd){
        idxTargetImage = imageSet.numImages-1;
        UseEnd=false;
    }else if(AdvanceRandom){
        idxTargetImage = rand()% imageSet.numImages;
    }else{
        idxTargetImage++;
        if(idxTargetImage==imageSet.numImages)
            idxTargetImage=0;
    }
    mosaic.build(idxTargetImage, &imageSet);
    z=1;
    drawStage=3;
    
    mosaicSet.buildSet(idxPre, idxTargetImage);
}
