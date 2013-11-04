//
//  SmartViewportManager.h
//  SmartViewportManager
//
//  Created by Kojo Acquah on 3/14/13.
//
//

#ifndef __SmartViewportManager__SmartViewportManager__
#define __SmartViewportManager__SmartViewportManager__

#include <iostream>
#include "ofMain.h"
#include "SmartLayerManager.h"
#include "SmartViewport.h"

class SmartViewportManager{
public:
    SmartViewportManager();
    SmartViewportManager(int viewportWidth, int viewportHeight);
    ~SmartViewportManager();
    
    SmartViewport* createViewpot(ofVec3f position,
                                 ofVec2f rotXY,
                                 int createrUserID,
                                 ofImage preview);
    
    // Draws all the viewports in the world. Assumses current
    // coordinate system starts from center (0,0,0)
    void drawViewportPreviews();
    
    // Returns the closest viewport to the position
    SmartViewport* closestViewport(ofVec3f position, float &length);
    
    // Smartviewport data
    vector<SmartViewport*> getSmartViewports();
    int getNumSmartViewports();
    
    // Determines where the center of the world for viewport
    // previews is drawn
    void setCenterOfWorldOffset(ofVec3f newCenter);
    
    void setviewportPreviewAlpha(int previewAlpha);
    int getViewportPreviewAlpha();
    
private:
    vector<SmartViewport*> smartViewports;
    int viewportWidth, viewportHeight;
    
    int viewportPreviewAlpha; 
    
    ofVec3f centerOfWorldOffset;
    
    // Offset between the camera position and where the viewport is
    // created.
    ofVec3f cameraToViewportOffset;
};
#endif /* defined(__SmartViewportManager__SmartViewportManager__) */
