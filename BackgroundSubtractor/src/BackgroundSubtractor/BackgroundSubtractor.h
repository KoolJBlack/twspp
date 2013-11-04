//
//  BackgroundSubtractor.h
//  SmartViewportManager
//
//  Created by Kojo Acquah on 3/17/13.
//
//

#ifndef __SmartViewportManager__BackgroundSubtractor__
#define __SmartViewportManager__BackgroundSubtractor__

#include <iostream>
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"

#define minArea (150)
#define maxArea ((800 * 600) / 2)
#define nBlobsConsidered (10)
#define bFindHoles (false)
#define bUseApproximation (true)

#define INITIAL_THRESHOLD (30)


class BackgroundSubtractor{
public:
    BackgroundSubtractor();
    BackgroundSubtractor(int imgWidth, int imgHeight);
    BackgroundSubtractor(int imgWidth, int imgHeight, ofRectangle newROI);
    ~BackgroundSubtractor();
    
    // Updates Background subtractor with new image
    void updateImg(ofxCvColorImage img);

    void setROI(ofRectangle newROI);
    ofRectangle getROI();
    
    void setThreshold(int newThreshold);
    int getThreshold();
    
    void setImageMirroring(bool newMirroring);
    
    // Returns the image ROI with the background as transparency
    ofImage getBackgroundSubtracktedImageROI();
    // Returns the whole image with the background as transparency
    ofImage getBackgroundSubtracktedImage();
    
    // Applies background subtraction to current camera image
    void zeroBackground();
    
private:
    
    int imgWidth;
    int imgHeight;
    
    // Threshold used for background subtracktion
    int threshold;
    
    // Set true to mirror input images
    bool mirroring;
    
    // Images used for image analysis and background subtraction
    ofxCvColorImage colorImg;
    ofxCvGrayscaleImage grayImage;
    ofxCvGrayscaleImage grayBg;
    ofxCvGrayscaleImage grayDiff;
    
    //ofxCvContourFinder contourFinder;
    
    // Indicates when background should be relearned
    bool bLearnBackground;
    
    // Indicates region used for capturing pixels
    ofRectangle ROI;
};

#endif /* defined(__SmartViewportManager__BackgroundSubtractor__) */
