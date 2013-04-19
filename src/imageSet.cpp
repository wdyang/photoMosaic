#include "imageSet.h"
#include "mosaicSet.h"

void ImageSet::addImage(ofxCvGrayscaleImage imageIn){
    
    grayImages.push_back(imageIn);
    ofxCvGrayscaleImage mid=imageIn;
    mid.resize(mid_width, mid_height);
    grayMids.push_back(mid);
    ofxCvGrayscaleImage icon = mid;
    icon.resize(icon_width, icon_height);
    grayIcons.push_back(icon);

    CvScalar avg = cvAvg(icon.getCvImage());
    imageMeans.push_back(avg.val[0]);
    

    numImages++;
}

void ImageSet::saveImage(ofxCvGrayscaleImage image){
    time_t now;
    char the_date[24];
    
    the_date[0] = '\0';
    
    now = time(NULL);
    
    if (now != -1)
    {
        strftime(the_date, 24, "%y_%m_%d_%H_%M_%S", gmtime(&now));
    }
    string fname;
    fname = "capture/"+string(the_date) + ".jpg";
    cout<<fname<<endl;
    
    ofImage tempImage;
    tempImage.setFromPixels(image.getPixels(), image.getWidth(), image.getHeight(), OF_IMAGE_GRAYSCALE);
    tempImage.saveImage(fname);
}

void ImageSet::loadImages(){
    dir.listDir("images/");
    dir.sort();
    int numFiles = dir.size();
    cout<<"loaded "<<numImages<<" images.";
    if(dir.size()){
        
        for(int i=0; i<numFiles; i++){
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
    ofxCvGrayscaleImage gray;
    gray = cvColor;
    addImage(gray);
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
        if(abs(val - imageMeans[i])+ofRandom(10)<diff){
            diff = abs(val-imageMeans[i]);
            idx =i;
        }
    }
//    cout<<"diff "<<diff<<" "<<endl;
    return idx;
}

