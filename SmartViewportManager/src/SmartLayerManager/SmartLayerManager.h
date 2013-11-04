//
//  SmartLayerManager.h
//  SmartLayerCapture
//
//  Created by Kojo Acquah on 1/25/13.
//
//

#ifndef SmartLayerCapture_SmartLayerManager_h
#define SmartLayerCapture_SmartLayerManager_h

#import "ofMain.h"

#include "SmartLayer.h"
#include "SmartShape.h"
#include "SmartPoly.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"

#include <vector.h>


class SmartLayerManager {
private:
    
    ofVideoGrabber vidGrabber;
    
    // Images used for image analysis and background subtraction
    ofxCvColorImage colorImg;
    ofxCvGrayscaleImage grayImage;
    ofxCvGrayscaleImage grayBg;
    ofxCvGrayscaleImage grayDiff;
    
    ofxCvContourFinder contourFinder;
    
    // Indicates when background should be relearned
    bool bLearnBackground;
    
    // Holds each smart layer. A smart layer stores the shapes and image snapshot
    vector<SmartLayer*> smartLayers;
    
    // Determnies how big smart layers draw themselves
    float smartLayerScale;
    
    // Converts a vector of of::ofPoint to cv::Point
    vector<cv::Point> convertofPointsTocvPoints(vector<ofPoint> ofPts);
    vector<ofPoint> convertcvPointsToofPoints(vector<cv::Point> cvPts);
    
    int selectedLayer;
    
public:
    SmartLayerManager();
    
    // Setup the layer manager with vid grabber. Must be done before
    // Layers can be captured.
    void initWithGrabber(ofVideoGrabber * grabber);
    // Setup the layer manager without vid grabber. User must pass in images during update cycles.
    void initWithoutGrabber(int width, int height);
    
    // Updates the SmartLayerManager with the latest camera image
    void update();
    void updateImg(ofxCvColorImage img);
    
    //Chigira added ROI img and function captureing only inside ROI
    ofxCvGrayscaleImage imgWithROI;
    void updateImg(ofxCvColorImage img, ofRectangle ROI, float rotAngle);
    //Chigira added end
    
    // Draws the vid grabber image at x, y
    void drawVidGrabber(int x = 0, int y = 0);
    // Draws the gray theshold difference image used for contour finding
    void drawGrayDiffThreshold(int x = 0, int y = 0);
    // Draws contours detected from the gray diff image
    void drawContours(int x = 0, int y = 0);
    
    // Draws only the smart shapes for a specific layer
    void drawLayerSmartShapes(int layerId, int x = 0, int y = 0);
    // Drawas only the smart shapes for layers within the range
    void drawLayerSmartShapesRange(int firstLayerId, int lastLayerId, int x = 0, int y = 0);
    // Draws only the smar shape overlays for a specific layer
    void drawLayerSmartShapeOverlays(int layerId, int x = 0, int y = 0);
    // Draws shape's extrusion for specifi layer
    void drawsLayerSmartMesh(int layerId, int x, int y);
    //void drawsLayerSmartMesh(int layerId, int x, int y, ofVec3f mkInfo[2], bool bChageExtrusionHeight[2], ofColor color[2]);
    // Draws the entire layer image. This will cover any layer images
    // drawn before this one.
    void drawLayerImage(int layerId, int x = 0, int y = 0);

    
    // Returns the number of smart layers in the manager
    int getNumSmartLayers();
    
    // Changes the rendering scale for smart layers
    void setSmartLayerScale(float s);
    
    // Takes a snapshot of the current camera image and creates a new layer
    // based on changes from the last layer snap shot.
    // This will also zero the background.
    void layerSnapShot();
    
    void layerSnapShot(int x, int y);//added for adding the captured pos
    
    // Applies background subtraction to current camera image
    void zeroBackground();
    // Erases all the layers stored in the SmartLayerManager
    void resetLayers();
    
    void layerSelectUp();
    void layerSelectDown();
    void layerSelect(int layerNum);
    int getSelectedLayerNum();
    
    SmartShape * onSelectShape(int layerID, int x, int y);
    
    SmartPoly * duplicateShapeInLayer(SmartPoly * shape, int layerIndex);
    
    void removeCopyFromLayer(SmartShape * shape, int layerIndex);

};


#endif
