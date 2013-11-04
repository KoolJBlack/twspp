//
//  BackgroundSubtractor.cpp
//  SmartViewportManager
//
//  Created by Kojo Acquah on 3/17/13.
//
//

#include "BackgroundSubtractor.h"
BackgroundSubtractor::BackgroundSubtractor(){
    
}


BackgroundSubtractor::BackgroundSubtractor(int imgWidth, int imgHeight){
    
    // Allocate memory for images
    colorImg.allocate(imgWidth, imgHeight);
    grayImage.allocate(imgWidth, imgHeight);
    grayBg.allocate(imgWidth, imgHeight);
    grayDiff.allocate(imgWidth, imgHeight);
    
    bLearnBackground = false;
    mirroring = false;
    threshold = INITIAL_THRESHOLD;
    
    this->imgWidth = imgWidth;
    this->imgHeight = imgHeight;
    
    ROI = ofRectangle(0,0, imgWidth,imgHeight);
}


BackgroundSubtractor::BackgroundSubtractor(int imgWidth, int imgHeight, ofRectangle newROI){
    
    // Allocate memory for images
    colorImg.allocate(imgWidth, imgHeight);
    grayImage.allocate(imgWidth, imgHeight);
    grayBg.allocate(imgWidth, imgHeight);
    grayDiff.allocate(imgWidth, imgHeight);
    
    bLearnBackground = false;
    mirroring = false;
    
    this->imgWidth = imgWidth;
    this->imgHeight = imgHeight;
    
    ROI = newROI;
}


BackgroundSubtractor::~BackgroundSubtractor(){
    
}


void BackgroundSubtractor::updateImg(ofxCvColorImage img){
    colorImg.setFromPixels(img.getPixels(),img.width,img.height);
    
    // Mirror image if mirroring is set
    if(mirroring){
        colorImg.mirror(true,true);
    }
    
    // convert our color image to a grayscale imag
    grayImage = colorImg;
    
    grayImage.blur(3); // Blure the image slightly
    
    // update the background image
    if (bLearnBackground == true) {
        grayBg = grayImage;
        bLearnBackground = false;
    }
    
    
    // Background diff and theshold
    grayDiff.absDiff(grayBg, grayImage);
    grayDiff.threshold(threshold);
    
    // Find contours in the differenced image
    /*
    contourFinder.findContours(grayDiff, minArea, maxArea, nBlobsConsidered, bFindHoles, bUseApproximation);
     */
}


void BackgroundSubtractor::setROI(ofRectangle newROI){
    ROI = newROI;
}


ofRectangle BackgroundSubtractor::getROI(){
    return ROI;
}


void BackgroundSubtractor::setThreshold(int newThreshold){
    threshold = ofClamp(newThreshold, 0, 255);
}


int BackgroundSubtractor::getThreshold(){
    return threshold;
}


void BackgroundSubtractor::setImageMirroring(bool newMirroring){
    mirroring = newMirroring;
}


ofImage BackgroundSubtractor::getBackgroundSubtracktedImageROI(){
    
    // Get ROI pixels of color image and graydiff
    grayDiff.setROI(ROI.x, ROI.y, ROI.width, ROI.height);
    unsigned char * diffPixels = grayDiff.getRoiPixels();
    colorImg.setROI(ROI.x, ROI.y, ROI.width, ROI.height);
    unsigned char * colorPixels = colorImg.getRoiPixels();
    
    // Turn ROI values into int (for use in loops)
    int bx = ROI.x;
    int by = ROI.y;
    int bw = ROI.width;
    int bh = ROI.height;
    
    // Determine nuber of pixels based on image ROI size
    int numPixels = ROI.width * ROI.height;
    
    // Color pixels with transparency output
    unsigned char * outputPixels = new unsigned char[numPixels * 4];
    
    for(int i = 0; i < bw; ++i){
        for(int j = 0; j < bh; ++j){
            // Set the color pixels
            int index = (bw * j + i);
            outputPixels[index*4]=colorPixels[index*3];
            outputPixels[index*4+1]=colorPixels[index*3+1];
            outputPixels[index*4+2]=colorPixels[index*3+2];
            
            // If threshold pixel is white, set pixel to opaque
            // else, make the pixel transparent
            if(diffPixels[index] > 200){
                outputPixels[index*4+3] = 255;
            }else{
                outputPixels[index*4+3] = 0;
            }
        }
    }
   
    // Reset ROIs for further processing 
    grayDiff.setROI(0, 0, imgWidth, imgHeight);
    colorImg.setROI(0, 0, imgWidth, imgHeight);

    
    ofImage outputImage;
    outputImage.setFromPixels(outputPixels, bw, bh, OF_IMAGE_COLOR_ALPHA);
    
    //TODO: Find the right way to fix this
    delete outputPixels;
    
    return outputImage;
}

ofImage BackgroundSubtractor::getBackgroundSubtracktedImage(){
    
    // Get pixels of color image and graydiff
    unsigned char * diffPixels = grayDiff.getRoiPixels();
    unsigned char * colorPixels = colorImg.getRoiPixels();
    
    // Integer values for image dimensions (for use in loops)
    int bx = 0;
    int by = 0;
    int bw = colorImg.width;
    int bh = colorImg.height;
    
    // Determine nuber of pixels based on image ROI size
    int numPixels = colorImg.width * colorImg.height;
    
    // Color pixels with transparency output
    unsigned char * outputPixels = new unsigned char[numPixels * 4];
    
    for(int i = 0; i < bw; ++i){
        for(int j = 0; j < bh; ++j){
            // Set the color pixels
            int index = (bw * j + i);
            outputPixels[index*4]=colorPixels[index*3];
            outputPixels[index*4+1]=colorPixels[index*3+1];
            outputPixels[index*4+2]=colorPixels[index*3+2];
            
            // If threshold pixel is white, set pixel to opaque
            // else, make the pixel transparent
            if(diffPixels[index] > 200){
                outputPixels[index*4+3] = 255;
            }else{
                outputPixels[index*4+3] = 0;
            }
        }
    }

    ofImage outputImage;
    outputImage.setFromPixels(outputPixels, bw, bh, OF_IMAGE_COLOR_ALPHA);
    
    //TODO: Find the right way to fix this
    delete outputPixels;
    
    return outputImage;
}



void BackgroundSubtractor::zeroBackground(){
    bLearnBackground = true;
}
