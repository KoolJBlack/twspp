//
//  SmartLayerManager.cpp
//  SmartLayerCapture
//
//  Created by Kojo Acquah on 1/25/13.
//
//

#include "SmartLayerManager.h"
#include "SmartPoly.h"

/*
#define minArea (150)
#define maxArea ((800 * 600) / 2)
#define nBlobsConsidered (20)
#define bFindHoles (false)
#define bUseApproximation (true)

#define THRESHOLD (15)
 */

#define minArea (150)
#define maxArea ((800 * 600) / 2)
#define nBlobsConsidered (10)
#define bFindHoles (false)
#define bUseApproximation (true)

#define THRESHOLD (30)

using namespace cv;
using namespace ofxCv;


SmartLayerManager::SmartLayerManager(){
    bLearnBackground = false;
    smartLayerScale = 1.0;
    
    selectedLayer = -1;
}

void SmartLayerManager::initWithGrabber(ofVideoGrabber * grabber){    
    vidGrabber = *grabber;
    
    smartLayers = vector<SmartLayer*>();
    
    // Allocate memory for images
    colorImg.allocate(vidGrabber.width, vidGrabber.height);
    grayImage.allocate(vidGrabber.width, vidGrabber.height);
    grayBg.allocate(vidGrabber.width, vidGrabber.height);
    grayDiff.allocate(vidGrabber.width, vidGrabber.height);
    
    bLearnBackground = false;
    
}


void SmartLayerManager::update(){
    //vidGrabber.grabFrame();
    vidGrabber.update();
    
    if (vidGrabber.isFrameNew()){
        // Get the new frame
        colorImg.setFromPixels(vidGrabber.getPixelsRef());
        
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
        grayDiff.threshold(THRESHOLD);
        
        // Find contours in the differenced image
        contourFinder.findContours(grayDiff, minArea, maxArea, nBlobsConsidered, bFindHoles, bUseApproximation);
    }
}


void SmartLayerManager::drawVidGrabber(int x, int y){
    colorImg.draw(x, y);
}

void SmartLayerManager::drawGrayDiffThreshold(int x, int y){
    grayDiff.draw(x,y);
}

void SmartLayerManager::drawContours(int x, int y){
    if( contourFinder.nBlobs){
        contourFinder.draw(x,y);
    }
}


void SmartLayerManager::drawLayerSmartShapes(int layerId, int x, int y){
    assert(layerId < smartLayers.size());
    assert(layerId >= 0);
    
    smartLayers[layerId]->drawSmartShapes(x,y, smartLayerScale);
}

void SmartLayerManager::drawLayerSmartShapesRange(int firstLayerId, int lastLayerId, int x, int y){
    assert(firstLayerId <= lastLayerId);
    assert(lastLayerId < smartLayers.size());
    assert(firstLayerId >= 0);
    
    for(int i = firstLayerId; i <= lastLayerId; ++i){
        smartLayers[i]->drawSmartShapes(x,y, smartLayerScale);
    }
}

void SmartLayerManager::drawLayerSmartShapeOverlays(int layerId, int x, int y){
    assert(layerId < smartLayers.size());
    assert(layerId >= 0);
    
    smartLayers[layerId]->drawSmartShapeOverlays(x,y, smartLayerScale);
}

void SmartLayerManager::drawLayerImage(int layerId, int x, int y){
    assert(layerId < smartLayers.size());
    assert(layerId >= 0);

    smartLayers[layerId]->drawLayerImage(x,y, smartLayerScale);
}


int SmartLayerManager::getNumSmartLayers(){
    return smartLayers.size();
}

void SmartLayerManager::setSmartLayerScale(float s){
    smartLayerScale = s;
}


vector<cv::Point> SmartLayerManager::convertofPointsTocvPoints(vector<ofPoint> ofPts){
    // Convert all ofPoint's to cv:Point's.
    vector<cv::Point> pts;
    for (vector<ofPoint>::iterator it = ofPts.begin() ; it != ofPts.end(); ++it){
        ofPoint p = *it;
        pts.push_back( cv::Point(p.x, p.y));
    }
    return pts;
}

vector<ofPoint> SmartLayerManager::convertcvPointsToofPoints(vector<cv::Point> cvPts){
    // Convert all ofPoint's to cv:Point's.
    vector<ofPoint> pts;
    for (vector<cv::Point>::iterator it = cvPts.begin() ; it != cvPts.end(); ++it){
        cv::Point p = *it;
        pts.push_back(ofPoint(p.x, p.y));
    }
    return pts;
}


void SmartLayerManager::layerSnapShot(){
    
    vector<SmartShape*> smartPolys = vector<SmartShape*>();
    
    // Create a SmartPoly for each new shape
    for(int i = 0; i < contourFinder.nBlobs; i++) {
        ofxCvBlob blob = contourFinder.blobs.at(i);
        
        // Convert all ofPoint's to cv:Point's.
        vector<cv::Point> pts = convertofPointsTocvPoints(blob.pts);
        
        // Create approxPoly from contour points
        vector<cv::Point> cvApproxpts;
        approxPolyDP(cv::Mat(pts), // InputArray curve
                     cvApproxpts, // OutputArray approxCurve
                     cv::arcLength(pts, true) * 0.04, // epsilon precision
                     true); // bool closed
        
        // Convert approxPionts to ofPionts
        vector<ofPoint> ofApproxPts = convertcvPointsToofPoints(cvApproxpts);
        
        //Add to smartPoly list
        SmartPoly* poly = new SmartPoly(ofApproxPts, 0, blob.boundingRect);
        //poly->setBoundingBox(blob.boundingRect);
        smartPolys.push_back(poly);
    }
    
    // Create the SmartLayer    
    SmartLayer* smartLayer = new SmartLayer(colorImg, grayDiff, smartPolys);
    // Initialize the shape images
    smartLayer->initShapeImages();
    
    // Add to SmartLayerManger
    smartLayers.push_back(smartLayer);
    
    // Re zero background
    zeroBackground();
}


void SmartLayerManager::zeroBackground(){
    bLearnBackground = true;
}

void SmartLayerManager::resetLayers(){
    smartLayers.clear();
}

void SmartLayerManager::layerSelectUp(){
    // if there are no layers to select, simply return
    if(!getNumSmartLayers()){
        return;
    }
    // Increment selected layer by 1
    ++selectedLayer;
    
    // Clamp selected layer
    selectedLayer = (int)ofClamp(selectedLayer, 0, getNumSmartLayers() - 1);
}

void SmartLayerManager::layerSelectDown(){
    // if there are no layers to select, simply return
    if(!getNumSmartLayers()){
        return;
    }
    // Decrement selected layer by 1
    --selectedLayer;
    
    // Clamp selected layer
    selectedLayer = (int)ofClamp(selectedLayer, 0, getNumSmartLayers() - 1);
}

int SmartLayerManager::getSelectedLayerNum(){
    return selectedLayer;
}

SmartShape * SmartLayerManager::onSelectShape(int x, int y){
    // Cannot select a shape without a selected layer
    if(selectedLayer == -1){
        return NULL;
    }
    
    // Return selected shape from layer
    return smartLayers[selectedLayer]->onShapeSelect(x, y);
}

SmartPoly* SmartLayerManager::duplicateShapeInLayer(SmartPoly *shape, int layerIndex){
    // Copy the shape
    SmartPoly * newShape = new SmartPoly();
    *newShape = *shape;
    
    // Add it to the layer
    smartLayers[layerIndex]->addSmartShape(newShape);
    
    // This new shape is a copy
    newShape->setIsCopy(true);
    
    // Return the new shape
    return newShape;
}

void SmartLayerManager::removeCopyFromLayer(SmartShape *shape, int layerIndex){
    if(!shape->getIsCopy()){
        throw new String("Cannot remove a non copy shape.");
    }
    
    // Remove it from the layer
    smartLayers[layerIndex]->removeSmartShape(shape);
}



