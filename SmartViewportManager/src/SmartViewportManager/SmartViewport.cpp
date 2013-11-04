//
//  SmartViewport.cpp
//  SmartViewportManager
//
//  Created by Kojo Acquah on 3/14/13.
//
//

#include "SmartViewport.h"

SmartViewport::SmartViewport(){
     
}

SmartViewport::SmartViewport(int createrUser,
                             ofImage preview,
                             ofVec3f pos,
                             ofVec2f rotXY,
                             int viewportWidth,
                             int viewPortHeight){
    creatorUserID = createrUser;
    viewportPreview = preview;
    
    position = pos;
    rotationXY = rotXY;
    
    // Resize the preview image
    viewportPreview.resize(viewportWidth, viewPortHeight);
    
    // Initialize the laer manager. 
    smartLayerManager.initWithoutGrabber(viewportWidth, viewPortHeight);
    
    // Set time of creation
    creationTime = time(NULL);
}

SmartViewport::~SmartViewport(){
    
}


SmartLayerManager* SmartViewport::getSmartLayerManager(){
    return &smartLayerManager;
}

ofImage* SmartViewport::getImagePreview(){
    return &viewportPreview;
}

void SmartViewport::setImagePreview(ofImage preview){
    viewportPreview = preview;
}

void SmartViewport::drawViewportPreview(int x, int y){
    viewportPreview.draw(x,y);
}


void SmartViewport::setPosition(ofVec3f pos){
    position = pos;
}

ofVec3f SmartViewport::getPosition(){
    return position;
}

ofVec2f SmartViewport::getRotationXY(){
    return rotationXY;
}

ofNode SmartViewport::getCamPose(){
    ofNode pose;
    pose.setPosition(getPosition());
    pose.setOrientation(ofVec3f(rotationXY.x, rotationXY.y + 180, 0));
    return pose;
}

int SmartViewport::getCreatorUserID(){
    return creatorUserID;
}

string SmartViewport::getCreationTimeString(){
    return ctime(&creationTime);
}




