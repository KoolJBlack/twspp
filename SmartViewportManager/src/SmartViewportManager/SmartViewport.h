//
//  SmartViewport.h
//  SmartViewportManager
//
//  Created by Kojo Acquah on 3/14/13.
//
//

#ifndef __SmartViewportManager__SmartViewport__
#define __SmartViewportManager__SmartViewport__

#include <iostream>
#include "ofMain.h"
#include "SmartLayerManager.h"

// For calculating the date and time
#include "time.h"


enum SmartViewportState{
    SMARTVIEWPORT_STATE_NORMAL,
    SMARTVIEWPORT_STATE_HIGHLIGHT,
    SMARTVIEWPORT_STATE_SELECTED,
};

class SmartViewport{
public:
    SmartViewport();
    SmartViewport(int createrUser,
                  ofImage preview,
                  ofVec3f pos,
                  ofVec2f rotXY,
                  int viewportWidth,
                  int viewPortHeight);
    ~SmartViewport();
    
    SmartLayerManager* getSmartLayerManager();
    
    ofImage* getImagePreview();
    void setImagePreview(ofImage preview);
    
    void drawViewportPreview(int x = 0, int y = 0);
    
    void setPosition(ofVec3f pos);
    void setRotationXY(ofVec2f ori);
    ofVec3f getPosition();
    ofVec2f getRotationXY();
    
    // Gives position for camera to be placed
    // in front of viewport
    ofNode getCamPose();
    
    int getCreatorUserID();
    string getCreationTimeString();
    
private:
    // Pose info
    ofVec3f position;
    ofVec2f rotationXY;
    
    //Meta Data
    int creatorUserID;
    time_t creationTime;
    
    // Smart Layer Manager data
    ofImage viewportPreview;
    SmartLayerManager smartLayerManager;
};

#endif /* defined(__SmartViewportManager__SmartViewport__) */
