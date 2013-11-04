//
//  WaypointCamera.cpp
//  WaypointCamera
//
//  Created by Kojo Acquah on 3/13/13.
//
//

#include "WaypointCamera.h"

WaypointCamera::WaypointCamera(){
    // Initialize the current and previous targets for cam
    curTarNode.setPosition(getPosition());
    pan(-180); // Fix for camera orientation
    curTarNode.setOrientation(getOrientationQuat());
    pan(180); // Fix for camera orientation 
    preTarNode = curTarNode;
    
    arrivedAtTarget = false;
    isMoving = false; 
}


WaypointCamera::~WaypointCamera(){
}


void WaypointCamera::updateCam(){
    
    // If the camera is in free mode, do not update its position
    if(camState == WAYPOINT_CAM_STATE_FREE){
        return;
    }
    
    // Calculate deltas between current position and target positoin
    ofVec3f posDelta = curTarNode.getPosition() - getPosition();
    ofVec3f oriDelta = curTarNode.getOrientationEuler() - getOrientationEuler();
    
    // Update pos if camera is too far from target
    if(posDelta.length() > CAM_VEL_RATE){
        isMoving = true;
        
        // Update positoin
        ofVec3f vel = posDelta.normalize();
        vel *= CAM_VEL_RATE;
        setPosition(getPosition() + vel);
        
        // Calculate fraction of distance from previous node
        float fromPreNodeDistFrac = (getPosition() - preTarNode.getPosition()).length()/(curTarNode.getPosition() - preTarNode.getPosition()).length();
        
        // Lerp between old and new orientations
        ofQuaternion newOri = quaternionLerp(preTarNode.getOrientationQuat(),
                                             curTarNode.getOrientationQuat(),
                                             fromPreNodeDistFrac);
        setOrientation(newOri);
        
        // Rotate 180 on Y axis so camera faces pos Z direction
        pan(180);
    }else{
        // Free the cam once we have reached our target
        camState =  WAYPOINT_CAM_STATE_FREE;
        isMoving = false;
        
        // We have arrived at target
        arrivedAtTarget = true; 
    }
}


void WaypointCamera::setTarget(ofNode target){
    curTarNode = target;
}


void WaypointCamera::setTarget(ofVec3f position, ofQuaternion orientation){
    curTarNode.resetTransform();
    curTarNode.setPosition(position);
    curTarNode.setOrientation(orientation);
}


ofNode WaypointCamera::getTarget(){
    return curTarNode;
}


void WaypointCamera::setSnapToTarget(bool snap){
    if(snap == true){
        camState = WAYPOINT_CAM_STATE_TARGET;
        
        // Update the prev target node
        preTarNode.resetTransform();
        preTarNode.setPosition(getPosition());
        pan(-180); // Fix for camera orientation
        preTarNode.setOrientation(getOrientationQuat());
        pan(180); // Fix for camera orientation
    }else{
        camState = WAYPOINT_CAM_STATE_FREE;
    }
}

bool WaypointCamera::getSnapToTarget(){
    return camState == WAYPOINT_CAM_STATE_TARGET ? true: false; 
}


bool WaypointCamera::getArrivedAtTarget(){
    if (arrivedAtTarget){
        arrivedAtTarget = false;
        return true;
    }else{
        return false; 
    }
}

bool WaypointCamera::getIsMoving(){
    return isMoving;
}


WaypointCamState WaypointCamera::getWaypointCamState(){
    return camState;
}


//--------------------------------------------------------------
// Helper Functionss
//--------------------------------------------------------------

ofQuaternion WaypointCamera::quaternionLerp(ofQuaternion start,
                                     ofQuaternion stop,
                                     float pct){
    // Ensure pct between 0 and 1.0
    pct = ofClamp(pct, 0, 1.0);
    ofQuaternion delta = stop - start;
    delta *= pct;
    return start + delta;
}