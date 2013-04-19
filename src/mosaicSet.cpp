#include "mosaicSet.h"


void Mosaic::build(int _idxImage, ImageSet* _imageSet){
    imageSet = _imageSet;
    idxImage = _idxImage;

    //first shrink image down
    ofxCvGrayscaleImage tempImage = imageSet->grayImages[idxImage];
    tempImage.resize(mosaicXSteps, mosaicYSteps);
    IplImage * cvImage =   tempImage.getCvImage();
    mosaic = imageSet->grayImages[idxImage];
    
    cout<< "image widthstep is "<<tempImage.getCvImage()->widthStep;
    unsigned char* pixels = tempImage.getPixels();
    for (int i=0; i<mosaicYSteps; i++){
        unsigned char* ptr = (unsigned char*)(cvImage->imageData + i * cvImage->widthStep);
        for (int j=0; j<mosaicXSteps; j++){
//            cout<<(int) ptr[j]<<"(";
            mosaicAvg[i][j]=(int) ptr[j];
            mosaicIdx[i][j] = imageSet->getMatchingIcon((int) ptr[j]);
//            cout<<mosaicIdx[i][j]<<")";
            mosaic.setROI(j*icon_width, i*icon_height, icon_width, icon_height);
            mosaic.setRoiFromPixels(imageSet->grayIcons[mosaicIdx[i][j]].getPixels(), icon_width, icon_height);
        }
//        cout<<endl;
    }
    //clear ROI
    mosaic.setROI(0,0,full_width,full_height);
}

void MosaicSet::init(ImageSet* _imageSet){
    imageSet = _imageSet;
    mosaicStageFullRes.allocate(5*full_width, 5*full_height);
    mosaicStageMidRes.allocate(40*mid_width, 40*mid_height);
}

void MosaicSet::buildSet(int _from, int _to){
    idxFromImage = _from;
    idxToImage = _to;
    
    //first build target mosaic
    mosaic.build(idxToImage, imageSet);
    
    //replace a center icon with _from
    float from_avg = imageSet->imageMeans[idxFromImage];
    float min_diff = 10000;
    for(int i=12; i< mosaicYSteps-10; i+=5){ //Y
        for(int j=12; j<mosaicXSteps-10; j+=5){ //X
            float diff = abs(mosaic.mosaicAvg[i][j]-from_avg);
            if (diff < min_diff){
                fromInTo_Y=i;
                fromInTo_X=j;
                min_diff=diff;
            }
        }
    }
//    fromInTo_Y = 20;
//    fromInTo_X = 20;
    
    mosaic.mosaicIdx[fromInTo_Y][fromInTo_X] = idxFromImage;
    
//    cout<<"fromInTo_Y:"<<fromInTo_Y<<" fromInTo_X:"<<fromInTo_X<<endl;
    
    //make a 5x5 full size mosaic with _from in the center
    for (int y=0; y<5; y++){
        for(int x=0; x<5; x++){
            mosaicStageFullRes.setROI(x*full_width, y*full_height, full_width, full_height);
            int imageid = mosaic.mosaicIdx[fromInTo_Y+y-2][fromInTo_X+x-2];
            mosaicStageFullRes.setRoiFromPixels(imageSet->grayImages[imageid].getPixels(), full_width, full_height);
        }
    }
    
    //make a 40x40 mid size image mosaic
    for (int y=0; y<mosaicYSteps; y++){
        for(int x=0; x<mosaicXSteps; x++){
            mosaicStageMidRes.setROI(x*mid_width, y*mid_height, mid_width, mid_height);
            int imageid = mosaic.mosaicIdx[y][x];
            mosaicStageMidRes.setRoiFromPixels(imageSet->grayMids[imageid].getPixels(), mid_width, mid_height);
        }
    }
    cout<<"build MosaicSet done!"<<endl;

}


