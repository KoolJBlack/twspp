//
//  SmartLayer.h/Users/kojo/Code/of_v0073_osx_release/apps/myApps/SmartLayerCapture/SmartLayerCapture.xcodeproj
//  SmartLayerCapture
//
//  Created by Kojo Acquah on 1/25/13.
//
//

#ifndef SmartLayerCapture_SmartLayer_h
#define SmartLayerCapture_SmartLayer_h

#import "ofMain.h"

#include "SmartShape.h"
#include "ofxOpenCv.h"

#include <vector.h>

class SmartLayer {
private:
    // Full image captured from camera
    ofxCvColorImage layerImage;
    // Gray threshold differenced image. Show's whats new in this frame.
    ofxCvGrayscaleImage grayDiffImage;
    // All smart shapes identified by smartshapefinder
    vector<SmartShape*> smartShapes;
    
public:
    SmartLayer();
    SmartLayer(ofxCvColorImage i, ofxCvGrayscaleImage g, vector<SmartShape*> s);
        
    // Uses layerImage and grayDiffImage to create a trasparent image for drawing
    // with alpha blending.
    void initShapeImages();
        
    int getNumSmartShapes();
    
    void addSmartShape(SmartShape * newShape);
    void removeSmartShape(SmartShape*  shape);
    
    vector<SmartShape*> getSmartShapes();
    
    ofxCvColorImage getLayerImage();
    ofxCvGrayscaleImage getGrayDiffImage();
    
    void drawSmartShapes(int x = 0, int y = 0, float scale = 1.0); // Only draws the smart shape images
    void drawSmartShapeOverlays(int x = 0, int y = 0, float scale = 1.0); // Draws the smart shape polygon overlays
    void drawLayerImage(int x = 0, int y = 0, float scale = 1.0); // Draws the whole image layer
    
    // Returns a pointer to a shape if it is selected, or NULL if no shapes is at coordinates x, y
    SmartShape * onShapeSelect(int x, int y);
};

#endif
