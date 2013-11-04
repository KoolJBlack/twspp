//
//  WaypointCamera.h
//  WaypointCamera
//
//  Created by Kojo Acquah on 3/13/13.
//
//

#ifndef __WaypointCamera__WaypointCamera__
#define __WaypointCamera__WaypointCamera__

#include <iostream>
#include "ofCamera.h"
#include "ofMain.h"

#define CAM_TO_TAR_POS_TOLERANCE (1.0)
#define CAM_VEL_RATE (5.0)
#define INITIAL_CAM_DISTANCE (650)

enum WaypointCamState{
    WAYPOINT_CAM_STATE_FREE,
    WAYPOINT_CAM_STATE_TARGET,
};


class WaypointCamera : public ofCamera{
public:
    WaypointCamera();
    ~WaypointCamera();
    
    // Update camera
    void updateCam();
    
    // Setup camera target
    void setTarget(ofNode target);
    void setTarget(ofVec3f position, ofQuaternion orientation);
    ofNode getTarget();
    
    // Sets the camera origin to the current camera pose
    void setOriginTarget();
    void setOriginTarget(ofNode target);
    void setOriginTarget(ofVec3f position, ofQuaternion orientation);
    ofNode getOriginTarget();
    
    // Sets the camera origin targit and moves the camera back to it
    void returnToOrigin();
    
    // Snap to target
    void setSnapToTarget(bool snap);
    bool getSnapToTarget();
    
    // Returns true only once for when the the camera
    // has arrived at its next target. If the camera has
    // arrived at two or more targets before this funciton
    // is called, true will only be returned once to indicate
    // the last target it arrived at. 
    bool getArrivedAtTarget();
    
    // Set Callback for target arrived. This is called
    // Whenever the WaypointCamera reaches it destination
    // void setTargetArrivedCallback(void (*callback)(void));
    
    // Camera status
    bool getIsMoving();
    WaypointCamState getWaypointCamState();
    
private:
    
    // Helpter functions
    ofQuaternion quaternionLerp(ofQuaternion start,
                                ofQuaternion stop,
                                float pct);
    
    // Holds state for the waypoint cam
    WaypointCamState camState;    
    
    // Current and previous camera target nodes
    ofNode curTarNode, preTarNode, originTarNode;
    
    // True when the camera is moving to next waypoint
    bool isMoving;
    
    // Set to true when the camera has arrived at its next target
    bool arrivedAtTarget;
    
};

#endif /* defined(__WaypointCamera__WaypointCamera__) */
