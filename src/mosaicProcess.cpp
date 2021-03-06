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
        if(!bStay){
            if(z>0){
                z-=speed/(hesitation*hesitation);
            }else{
                advance();
            }
        }
    }
    if(drawStage==1){
        transition_mosaic2gray(speed);
    }else if (drawStage==2){
        transition_mid2mosaic(speed);
    }else if(drawStage==3){
        transition_from2mid(speed);
//        displayImage.brightnessContrast(brightness, contrast);
    }

}

void MosaicProcess::transition_mosaic2gray(float delta){
//    drawStage is 1
    if(bZoomOut){ //Mosaic to Gray
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
    }else{ //Zoom in //Gray to Mosaic
        if(z<1){
            displayImage = mosaic.mosaic;
            ofxCvGrayscaleImage img_source = imageSet.grayImages[idxTargetImage];
            
            displayImage.convertToRange(0,  int(255*z));
            img_source.convertToRange(0, int(255*(1-z)));
            displayImage+=img_source;
            z+=delta/2;
        }else{
            cout<<"mosaic 2 gray done, showing gray image now"<<endl;
            drawStage++;
            z=8;
        }
        
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
    
    if(bZoomOut){ //mid images to full mosaic
        z *= (1+delta/5);
        if(z<=8){ //z starts at 1, ends at 8
            w = w0*z;
            h = h0*z;

            xc=int((xc1-xc0)*pow((w-w0)*1.0/(w1-w0), 2.0)) + xc0;         //slow then fast
            yc=int((yc1-yc0)*pow((h-h0)*1.0/(h1-h0), 2.0)) + yc0;
            
            x=xc-w/2; x= x < 0 ? 0 : x; x = (x+w) > w1 ? 0 : x;
            y=yc-h/2; y= y < 0 ? 0 : y; y = (y+h) > h1 ? 0 : y;
            
            mosaicSet.mosaicStageMidRes.setROI(x, y, w, h);
            displayImage.allocate(w, h);
            displayImage.setFromPixels(mosaicSet.mosaicStageMidRes.getRoiPixels(), w, h);
            displayImage.resize(full_width, full_height);
        }else{
            cout<<"mid 2 mosaic done, transition to mosaic 2 gray"<<endl;
            drawStage--;
            z=1;
        }
    }else{ //full mosaic to mid images
        z /= (1+delta/5);
        if(z>1){ //z starts at 1, ends at 8
            w = w0*z;
            h = h0*z;
            
            xc=int((xc1-xc0)*pow((w-w0)*1.0/(w1-w0), 2.0)) + xc0;         //slow then fast
            yc=int((yc1-yc0)*pow((h-h0)*1.0/(h1-h0), 2.0)) + yc0;
            
            x=xc-w/2; x= x < 0 ? 0 : x; x = (x+w) > w1 ? 0 : x;
            y=yc-h/2; y= y < 0 ? 0 : y; y = (y+h) > h1 ? 0 : y;
            
            mosaicSet.mosaicStageMidRes.setROI(x, y, w, h);
            displayImage.allocate(w, h);
            displayImage.setFromPixels(mosaicSet.mosaicStageMidRes.getRoiPixels(), w, h);
            displayImage.resize(full_width, full_height);
        }else{
            cout<<"mid 2 mosaic done, transition to mosaic 2 gray"<<endl;
            drawStage++;
            z=5;
        }
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
    if(bZoomOut){ //full image to mid mosaic
        z *= (1+delta*slowStartRatio);  //z starts from 1, ends at 5
        if(slowStartRatio<1) slowStartRatio*=1.01;
        if(z<=5){
            w = w0*z;
            h = h0*z;
            x= int((w1 - w)/2.0);
            y= int((h1-h)/2.0);
            mosaicSet.mosaicStageFullRes.setROI(x,y,w,h);
            displayImage.allocate(w, h);
            displayImage.setFromPixels(mosaicSet.mosaicStageFullRes.getRoiPixels(), w, h);
            displayImage.resize(full_width, full_height);
        }else{
            slowStartRatio = 0.1;
            drawStage--;
            z=1;
        }
    }else{ //mid mosaic to full image
        z /= (1+delta);  //z starts from 1, ends at 5
        if(z>1){
            w = w0*z;
            h = h0*z;
            x= int((w1 - w)/2.0);
            y= int((h1-h)/2.0);
            mosaicSet.mosaicStageFullRes.setROI(x,y,w,h);
            displayImage.allocate(w, h);
            displayImage.setFromPixels(mosaicSet.mosaicStageFullRes.getRoiPixels(), w, h);
            displayImage.resize(full_width, full_height);
        }else{
            drawStage=0;
            z=1;
        }
    }
}

void MosaicProcess::draw(){
    imageSet.showIcons(1050, 520,  20);
    showImage = displayImage;
    showImage.brightnessContrast(brightness, contrast);
    showImage.draw(5+(max_display_w-display_w)/2, 5+(max_display_h-display_h)/2, display_w, display_h);
    imageSet.grayImages[idxTargetImage].draw(1050,260, 320, 240); //, mid_width, mid_height);
    
}

void MosaicProcess::advance(){
    if(bZoomOut){ //Zooming out, previous image becomes a part of new image
        int idxPre = idxTargetImage; //Pre is in the center of TargetImage
        if(bUseVideo){
            imageSet.addImage(imageSet.tempGrayImage);
            imageSet.saveImage(imageSet.tempGrayImage);
            idxTargetImage = imageSet.numImages-1;
        }else if(bUseEnd){
            idxTargetImage = imageSet.numImages-1;
            bUseEnd=false;
        }else if(AdvanceRandom){
            idxTargetImage = bUsePresetOnly ? rand()% imageSet.numPresetImages : rand() % imageSet.numImages;
//                idxTargetImage = rand()% imageSet.numImages;
        }else{
            idxTargetImage++;
            if(idxTargetImage==imageSet.numImages)
                idxTargetImage=0;
        }
        mosaic.build(idxTargetImage, &imageSet);
        z=1;
        drawStage=3;
        
        mosaicSet.buildSet(idxPre, idxTargetImage);
    }else{ //Zomming in, previous image becomes a mosaic that contains the new image
        idxTargetImage=idxPre;
        if(bUseVideo){
            imageSet.addImage(imageSet.tempGrayImage);
            imageSet.saveImage(imageSet.tempGrayImage);
            idxPre = imageSet.numImages-1;
        }else if(bUseEnd){
            idxPre = imageSet.numImages-1;
            bUseEnd=false;
        }else if(AdvanceRandom){
            idxPre = bUsePresetOnly ? rand()% imageSet.numPresetImages : rand() % imageSet.numImages;
        }else{
            idxPre++;
            if(idxPre==imageSet.numImages)
                idxPre=0;
        }
        mosaic.build(idxTargetImage, &imageSet);
        z=0;
        drawStage=1;
        
        mosaicSet.buildSet(idxPre, idxTargetImage);
    }
}
