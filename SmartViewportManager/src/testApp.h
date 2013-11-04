#pragma once

#include "ofMain.h"
#include "WaypointCamera.h"
#include "SmartViewportManager.h"


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

    // SmartViewportManager
    void initDefaultViewports();
    
    // Camera Update Methods
    void updateCamTarget();
    
    ofNode closestTarget();
    void toggleCamSnapTarget();
    
    // Helper Methods
    void rotateToNormal(ofVec3f normal);
    ofVec3f vect3fLerp(ofVec3f start, ofVec3f stop, float pct);
    ofQuaternion quaternionLerp(ofQuaternion start,
                                ofQuaternion stop,
                                float pct);
    //ofQuaternion OrientationVecToQuat(ofVec3f orientation);
    
    // Path drawing
    deque<ofVec3f> pathVertices;
    ofMesh pathLines;
    ofVec3f previous, current;

    // SmartViewportManager
    SmartViewportManager smartViewportManager;
    
    //Cam member Variables
    WaypointCamera cam;

    // Current and previous camera target nodes
    ofNode curTarNode;
    deque<ofNode> targetNodes;

    // True when the camera is moving to next waypoint
    bool isMoving;

};
