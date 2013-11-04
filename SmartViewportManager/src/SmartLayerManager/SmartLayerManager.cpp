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
#define THRESHOLDROI (130)

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
    imgWithROI.allocate(vidGrabber.width,vidGrabber.height);
    
    bLearnBackground = false;
    
}

/*CHIGIRA ADDED*/
void SmartLayerManager::initWithoutGrabber(int width, int height){
    smartLayers = vector<SmartLayer*>();
    
    // Allocate memory for images
    colorImg.allocate(width, height);
    grayImage.allocate(width, height);
    grayBg.allocate(width, height);
    grayDiff.allocate(width, height);
    imgWithROI.allocate(width,height);
    
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

void SmartLayerManager::updateImg(ofxCvColorImage img) {
    colorImg.setFromPixels(img.getPixels(),img.width,img.height);
    
//    colorImg.mirror(true,true);
    
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

void SmartLayerManager::updateImg(ofxCvColorImage img, ofRectangle ROI, float rotAngle) {

    colorImg.setFromPixels(img.getPixels(),img.width,img.height);//set pixels    
    grayImage = colorImg;//color to gray
    imgWithROI = grayImage;//full to ROI
    imgWithROI.rotate(rotAngle, ROI.x, ROI.y);//set rotation
    imgWithROI.setROI(ROI);//set ROI
    imgWithROI.blur(5);//blur for threshold
    imgWithROI.threshold(THRESHOLDROI, true);//threshold
    
    // Find contours inside ROI
    contourFinder.findContours(imgWithROI, minArea, maxArea, nBlobsConsidered, bFindHoles, bUseApproximation);
    
    imgWithROI.resetROI();//reset ROI
    imgWithROI.rotate(-rotAngle, ROI.x, ROI.y);//reset rotation(necesary?)
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

/*CHIGIRA ADDED----------------------------------*/
void SmartLayerManager::drawsLayerSmartMesh(int layerId, int x, int y) {
    assert(layerId < smartLayers.size());
    assert(layerId >= 0);
    smartLayers[layerId]->drawSmartMesh(x, y, smartLayerScale);
}

/*
void SmartLayerManager::drawsLayerSmartMesh(int layerId, int x, int y, ofVec3f mkInfo[2], bool bChangeExtrusionHeight[2], ofColor color[2]) {
    
    assert(layerId < smartLayers.size());
    assert(layerId >= 0);
    smartLayers[layerId]->drawSmartMesh(x, y, smartLayerScale, mkInfo, bChangeExtrusionHeight, color);    
}
 */





/*CHIGIRA ADDED END------------------------------*/



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

void SmartLayerManager::layerSnapShot(int x, int y){
    
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
        ofPoint dst;
        dst.x = x;
        dst.y = y;
        SmartPoly* poly = new SmartPoly(ofApproxPts, 0, blob.boundingRect, dst);
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

void SmartLayerManager::layerSelect(int layer){
    // if there are no layers to select, simply return
    if(!getNumSmartLayers()){
        return;
    }

    selectedLayer = (int)ofClamp(layer, 0, getNumSmartLayers() - 1);
}
int SmartLayerManager::getSelectedLayerNum(){
    return selectedLayer;
}

SmartShape * SmartLayerManager::onSelectShape(int layerID, int x, int y){
    assert(layerID >= 0);
    assert(layerID < getNumSmartLayers());
    
    // Return selected shape from layer
    return smartLayers[layerID]->onShapeSelect(x, y);
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



