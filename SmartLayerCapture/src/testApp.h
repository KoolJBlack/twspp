#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"

#include "SmartLayerManager.h"

class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofVideoGrabber vidGrabber;
    SmartLayerManager smartLayerManager;
    
    // Pointer to shape returned by layer manger when doing shape selected
    SmartShape * selectedShape;
    // Indicates a shape has been selected (as apposed to just highlighted)
    bool selected;
    
    // For cycling through layers
    int layerOffset;
    
};
