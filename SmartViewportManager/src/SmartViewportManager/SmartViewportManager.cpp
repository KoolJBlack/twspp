//
//  SmartViewportManager.cpp
//  SmartViewportManager
//
//  Created by Kojo Acquah on 3/14/13.
//
//

#include "SmartViewportManager.h"

#define CAMERA_TO_VIEWPORT_Z_OFFSET (-100)

SmartViewportManager::SmartViewportManager(){
}

SmartViewportManager::SmartViewportManager(int viewportWidth, int viewportHeight){
    this->viewportWidth = viewportWidth;
    this->viewportHeight = viewportHeight;
    
    viewportPreviewAlpha = 150;
    
    cameraToViewportOffset = ofVec3f(0,0,CAMERA_TO_VIEWPORT_Z_OFFSET);
}

SmartViewportManager::~SmartViewportManager(){
    
}

SmartViewport* SmartViewportManager::createViewpot(ofVec3f position, ofVec2f rotXY, int createrUserID, ofImage preview){
    SmartViewport* newViewport = new SmartViewport(createrUserID,
                                                   preview,
                                                   position,
                                                   rotXY,
                                                   viewportWidth,
                                                   viewportHeight);
    
    smartViewports.push_back(newViewport);
    return newViewport;
}

void SmartViewportManager::drawViewportPreviews(){
    ofPushMatrix();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetColor(255, 255, 255, viewportPreviewAlpha);
    // Move to center of world offset
    ofTranslate(centerOfWorldOffset);
    
    for (vector<SmartViewport*>::iterator it = smartViewports.begin() ; it != smartViewports.end(); ++it){
        SmartViewport* smartViewport = *it;
        ofPushMatrix();
        ofTranslate(smartViewport->getPosition());
        ofVec2f rotXY = smartViewport->getRotationXY();
        ofRotateY(rotXY.y);
        ofRotateX(rotXY.x);
        
        // Apply the camera to viewport offset
        ofTranslate(cameraToViewportOffset);
        
        // Fix upsidedonw image issue
        ofRotateX(180);
        
        smartViewport->drawViewportPreview();
        ofPopMatrix();
    }
    
    ofDisableAlphaBlending();
    ofPopStyle();
    ofPopMatrix();
}

SmartViewport* SmartViewportManager::closestViewport(ofVec3f position, float &length){
    // If there are no waypoints, return null
    if(!smartViewports.size()){
        return NULL;
    }
    
    float closestDist = -1;
    SmartViewport* closestViewport;
    
    for (vector<SmartViewport*>::iterator it = smartViewports.begin() ; it != smartViewports.end(); ++it){
        SmartViewport* viewport = *it;
        
        if(closestDist == -1){
            closestDist = (position - viewport->getPosition()).length();
            closestViewport = viewport;
            continue;
        }
        
        float distance = (position- viewport->getPosition()).length();
        if(distance < closestDist){
            closestDist = distance;
            closestViewport = viewport;
        }
    }
    
    return closestViewport;
}

vector<SmartViewport*> SmartViewportManager::getSmartViewports(){
    return smartViewports;
}

int SmartViewportManager::getNumSmartViewports(){
    return smartViewports.size();
}

void SmartViewportManager::setCenterOfWorldOffset(ofVec3f newCenter){
    centerOfWorldOffset = newCenter;
}


void SmartViewportManager::setviewportPreviewAlpha(int previewAlpha){
    viewportPreviewAlpha = (int)ofClamp(viewportPreviewAlpha, 0, 255);
}

int SmartViewportManager::getViewportPreviewAlpha(){
    return viewportPreviewAlpha;
}







