#pragma once

#include "ofMain.h"
#include "WaypointCamera.h"

/*
enum WaypointCamState{
    WAYPOINT_CAM_STATE_FREE,
    WAYPOINT_CAM_STATE_TARGET,
};
 */

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    // Camera Update Methods
    void updateCam();
    void updateCamTarget();
    
    void CamTargetArrived();
    
    ofNode closestTarget();
    void toggleCamSnapTarget();
    
    // Helper Methods
    void rotateToNormal(ofVec3f normal);
    ofVec3f vect3fLerp(ofVec3f start, ofVec3f stop, float pct);
    ofQuaternion quaternionLerp(ofQuaternion start,
                                ofQuaternion stop,
                                float pct);
    
    deque<ofVec3f> pathVertices;
    ofMesh pathLines;
    ofVec3f previous, current;

    WaypointCamState camState;
    WaypointCamera cam;
    

    // Current and previous camera target nodes
    ofNode curTarNode, preTarNode;
    deque<ofNode> targetNodes;

};
