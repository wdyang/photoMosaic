#include "imageSet.h"
#include "mosaicSet.h"

void ImageSet::addImage(ofxCvGrayscaleImage imageIn){
//    if(numImages> grayImages.size()-3){
//        
//    }
    int i = numImages;
    grayImages[i] = imageIn;
    grayMids[i]=grayImages[i];
    grayMids[i].resize(mid_width, mid_height);
    grayIcons[i]=grayMids[i];
    grayIcons[i].resize(icon_width, icon_height);
    
    CvScalar avg = cvAvg(grayIcons[i].getCvImage());
    imageMeans[i] = avg.val[0];

    numImages++;
}

void ImageSet::loadImages(){
    dir.listDir("images/");
    dir.sort();
    numImages = dir.size();
    cout<<"loaded "<<numImages<<" images.";
    if(dir.size()){
        grayImages.assign(100+numImages, ofxCvGrayscaleImage());
        grayMids.assign(100+numImages, ofxCvGrayscaleImage());
        grayIcons.assign(100+numImages, ofxCvGrayscaleImage());
        imageMeans.assign(100+numImages, float());
        
        for(int i=0; i<numImages; i++){
            cout<<"reading image: "<<dir.getPath(i);
            setImageFromFile(i,dir.getPath(i));
        }
    }
}

void ImageSet::setImageFromFile(int i, string fname){
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
    grayMids[i]=grayImages[i];
    grayMids[i].resize(mid_width, mid_height);
    grayIcons[i]=grayMids[i];
    grayIcons[i].resize(icon_width, icon_height);
    
    CvScalar avg = cvAvg(grayIcons[i].getCvImage());
    imageMeans[i] = avg.val[0];
    //    cout<<"image "<<i<<" has average of "<<imageMeans[i];
    
}

void ImageSet::showIcons(int x, int y, int numCol){
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

int ImageSet::getMatchingIcon(int val){
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

