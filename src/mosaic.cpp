#include "mosaic.h"

void Mosaic::setup(){
    loadImages();
    
    buildMosaicImage(targetImage);
    displayImage = mosaicImage;
    z=1;

    
}

void Mosaic::update(){
    if(z>0){
        displayImage = mosaicImage;
        ofxCvGrayscaleImage img_source = grayImages[targetImage];
        
        displayImage.convertToRange(0,  int(255*z));
        img_source.convertToRange(0, int(255*(1-z)));
        displayImage+=img_source;
        z-=0.001;
    }

}

void Mosaic::draw(){
    showIcons(640, 0,  10);
    grayImages[targetImage].draw(0,0);
    displayImage.draw(0, 480);
    
}

void Mosaic::loadImages(){
    dir.listDir("images/");
    dir.sort();
    numImages = dir.size();
    cout<<"loaded "<<numImages<<" images.";
    if(dir.size()){
        grayImages.assign(numImages, ofxCvGrayscaleImage());
        grayIcons.assign(numImages, ofxCvGrayscaleImage());
        imageMeans.assign(numImages, float());
        
        for(int i=0; i<numImages; i++){
            cout<<"reading image: "<<dir.getPath(i);
            setImageFromFile(i,dir.getPath(i));
        }
    }
}

void Mosaic::setImageFromFile(int i, string fname){
    ofImage tempImage;
    tempImage.loadImage(fname);
    
    float full_ratio = (float)full_width/ (float)full_height;
    cout<<"full_ratio is "<<full_ratio;
    
    int h = tempImage.getHeight();
    int w = tempImage.getWidth();
    float ratio = (float)w / (float) h;
    if(ratio >= full_ratio){   //image is wider than needed, crop width from center
        int new_width = (int)(ratio * full_height);
        tempImage.resize(new_width, full_height);
        int x0 = (new_width - full_width)/2;
        tempImage.crop(x0, 0, full_width, full_height);
    }else{   //image is higher than needed, crop height from center
        int new_height = (int) full_width / ratio;
        tempImage.resize(full_width, new_height);
        int y0 = (new_height - full_height)/2;
        tempImage.crop(0, y0, full_width, full_height);
    }
    
    ofxCvColorImage cvColor;
    cvColor.setFromPixels(tempImage.getPixels(), full_width, full_height);
    grayImages[i] = cvColor;
    grayIcons[i]=grayImages[i];
    grayIcons[i].resize(icon_width, icon_height);
    
    CvScalar avg = cvAvg(grayIcons[i].getCvImage());
    imageMeans[i] = avg.val[0];
    //    cout<<"image "<<i<<" has average of "<<imageMeans[i];
    
}

void Mosaic::showIcons(int x, int y, int numCol){
    int row = 0, col = 0;
    for(int i=0; i<numImages; i++){
        grayIcons[i].draw(x+col*icon_width, y+row*icon_height);
        col++;
        if(col>=numCol){
            row++;
            col=0;
        }
    }
    
}

void Mosaic::buildMosaicImage(int targetImageIdx){
    //first shrink image down
    ofxCvGrayscaleImage tempImage = grayImages[targetImageIdx];
    tempImage.resize(mosaicX, mosaicY);
    IplImage * cvImage =   tempImage.getCvImage();
    mosaicImage = grayImages[targetImage];
    
    cout<< "image widthstep is "<<tempImage.getCvImage()->widthStep;
    unsigned char* pixels = tempImage.getPixels();
    for (int i=0; i<mosaicY; i++){
        unsigned char* ptr = (unsigned char*)(cvImage->imageData + i * cvImage->widthStep);
        for (int j=0; j<mosaicX; j++){
            cout<<(int) ptr[j]<<"(";
            mosaicAvg[i][j]=(int) ptr[j];
            mosaicIdx[i][j] = getMatchingIcon((int) ptr[j]);
            cout<<mosaicIdx[i][j]<<")";
            mosaicImage.setROI(j*icon_width, i*icon_height, icon_width, icon_height);
            mosaicImage.setRoiFromPixels(grayIcons[mosaicIdx[i][j]].getPixels(), icon_width, icon_height);
        }
        cout<<endl;
    }
    //clear ROI
    mosaicImage.setROI(0,0,full_width,full_height);
    
}

int Mosaic::getMatchingIcon(int val){
    float diff = 1000000;
    int idx;
    for(int i=0; i<numImages; i++){
        if(abs(val - imageMeans[i])<diff){
            diff = abs(val-imageMeans[i]);
            idx =i;
        }
    }
    return idx;
}

