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
    if(drawStage==0){ //showing the gray image
        if(z>0){
            z-=0.01;
        }else{
            advance();
        }
    }
    if(drawStage==1){
        transition_mosaic2gray(speed);
    }else if (drawStage==2){
        transition_mid2mosaic(speed);
    }else if(drawStage==3){
        transition_from2mid(speed);
    }
}

void MosaicProcess::transition_mosaic2gray(float delta){
//    drawStage is 1
    if(z>0){
        displayImage = mosaic.mosaic;
        ofxCvGrayscaleImage img_source = imageSet.grayImages[idxTargetImage];
        
        displayImage.convertToRange(0,  int(255*z));
        img_source.convertToRange(0, int(255*(1-z)));
        displayImage+=img_source;
        z-=delta/2;
    }else{
        cout<<"mosaic 2 gray done, showing gray image now"<<endl;
        drawStage--;
        z=1;
    }
    
}

void MosaicProcess::transition_mid2mosaic(float delta){
//    drawStage is 2
    //    start, ROI set at
    int x0=(mosaicSet.fromInTo_X-2)*mid_width;
    int y0=(mosaicSet.fromInTo_Y-2)*mid_height;
    int w0 = full_width;
    int h0 = full_height;
    int xc0=x0+w0/2;
    int yc0=y0+h0/2;
    int x1=0;
    int y1=0;
    int w1 = 8*full_width;
    int h1 = 8*full_height;
    int xc1=w1/2;
    int yc1=h1/2;
    int x,y,w, h, xc, yc;
    z *= (1+delta/5);
    if(z<=8){ //z starts at 1, ends at 8
        w = w0*z;
        h = h0*z;

//        xc=int((xc1-xc0)*(w-w0)*1.0/(w1-w0)) + xc0;       //linear shift center
//        yc=int((yc1-yc0)*(h-h0)*1.0/(h1-h0)) + yc0;
        xc=int((xc1-xc0)*pow((w-w0)*1.0/(w1-w0), 2.0)) + xc0;         //slow then fast
        yc=int((yc1-yc0)*pow((h-h0)*1.0/(h1-h0), 2.0)) + yc0;
        
        x=xc-w/2; x= x < 0 ? 0 : x; x = (x+w) > w1 ? 0 : x;
        y=yc-h/2; y= y < 0 ? 0 : y; y = (y+h) > h1 ? 0 : y;
        
//        x=int(z1*x0 + (1-z1)*x1);
//        y=int(z1*y0 + (1-z1)*y1);
//        w=int(z1*w0 + (1-z1)*w1);
//        h=int(z1*h0 + (1-z1)*h1);
        
        mosaicSet.mosaicStageMidRes.setROI(x, y, w, h);
        displayImage.allocate(w, h);
        displayImage.setFromPixels(mosaicSet.mosaicStageMidRes.getRoiPixels(), w, h);
        displayImage.resize(full_width, full_height);
//        z*=(1-delta);
    }else{
        cout<<"mid 2 mosaic done, transition to mosaic 2 gray"<<endl;
        drawStage--;
        z=1;
    }
}

void MosaicProcess::transition_from2mid(float delta){
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
    z *= (1+delta);  //z starts from 1, ends at 5
    if(z<=5){
        w = w0*z;
        h = h0*z;
        x= int((w1 - w)/2.0);
        y= int((h1-h)/2.0);
//        x=int(z1*x0 + (1-z1)*x1);
//        y=int(z1*y0 + (1-z1)*y1);
//        w=int(z1*w0 + (1-z1)*w1);
//        h=int(z1*h0 + (1-z1)*h1);
        
        mosaicSet.mosaicStageFullRes.setROI(x,y,w,h);
        displayImage.allocate(w, h);
        displayImage.setFromPixels(mosaicSet.mosaicStageFullRes.getRoiPixels(), w, h);
        displayImage.resize(full_width, full_height);
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
