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
            cout<<(int) ptr[j]<<"(";
            mosaicAvg[i][j]=(int) ptr[j];
            mosaicIdx[i][j] = imageSet->getMatchingIcon((int) ptr[j]);
            cout<<mosaicIdx[i][j]<<")";
            mosaic.setROI(j*icon_width, i*icon_height, icon_width, icon_height);
            mosaic.setRoiFromPixels(imageSet->grayIcons[mosaicIdx[i][j]].getPixels(), icon_width, icon_height);
        }
        cout<<endl;
    }
    //clear ROI
    mosaic.setROI(0,0,full_width,full_height);
}

void MosaicSet::init(ImageSet* _imageSet){
    imageSet = _imageSet;
}

void MosaicSet::buildSet(int _from, int _to){
    idxFromImage = _from;
    idxToImage = _to;
    
}


