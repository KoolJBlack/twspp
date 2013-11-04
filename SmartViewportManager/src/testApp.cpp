#include "testApp.h"
#include "ofCamera.h"

#define kMoveInc 20
#define kRotInc 5

#define INITIAL_CAM_DISTANCE (650)

#define VEIWPORT_WIDTH (360)
#define VIEWPORT_HEIGHT (240)

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	// draw the vertices in pathLines as a line strip
	pathLines.setMode(OF_PRIMITIVE_LINE_STRIP);
    
    // this sets the camera's distance from the object
    cam.setPosition(0, 0, INITIAL_CAM_DISTANCE);
    // Update the camera origin
    cam.setOriginTarget();
    
    smartViewportManager = SmartViewportManager(VEIWPORT_WIDTH,
                                                VIEWPORT_HEIGHT);
    
    initDefaultViewports();
    
}

void testApp::initDefaultViewports(){
    ofImage preview1;
    ofImage preview2;
    ofImage preview3;
    ofImage preview4;
    ofImage preview5;
    ofImage preview6;

    preview1.loadImage("overlayImg0.jpg");
    preview2.loadImage("overlayImg1.jpg");
    preview3.loadImage("overlayImg2.jpg");
    preview4.loadImage("overlayImg3.jpg");
    preview5.loadImage("overlayImg4.jpg");
    preview6.loadImage("overlayImg5.jpg");


    
    smartViewportManager.createViewpot(ofVec3f(0,0,0),
                                       ofVec2f(0,0),
                                       0,
                                       preview1);
    
    smartViewportManager.createViewpot(ofVec3f(-500,0,0),
                                       ofVec2f(0,90),
                                       0,
                                       preview2);
    
    smartViewportManager.createViewpot(ofVec3f(500,0,0),
                                       ofVec2f(0,0),
                                       0,
                                       preview3);
    
    smartViewportManager.createViewpot(ofVec3f(0,0,-700),
                                       ofVec2f(0,-45),
                                       0,
                                       preview4);
    
    smartViewportManager.createViewpot(ofVec3f(0,500,0),
                                       ofVec2f(30,0),
                                       0,
                                       preview5);
    
    smartViewportManager.createViewpot(ofVec3f(200,200,200),
                                       ofVec2f(30,30),
                                       0,
                                       preview6);
}

//--------------------------------------------------------------
void testApp::update(){
	previous = current;
	
	// generate a noisy 3d position over time
	float t = (2 + ofGetElapsedTimef()) * .10;
	current.x = ofSignedNoise(t, 0, 0);
	current.y = ofSignedNoise(0, t, 0);
	current.z = ofSignedNoise(0, 0, t);
	current *= 400; // scale from -1,+1 range to -400,+400
	
	// add the current position to the pathVertices deque
	pathVertices.push_back(current);
	// if we have too many vertices in the deque, get rid of the oldest ones
	while(pathVertices.size() > 200) {
		pathVertices.pop_front();
	}
	
	// clear the pathLines ofMesh from any old vertices
	pathLines.clear();
	// add all the vertices from pathVertices
	for(int i = 0; i < pathVertices.size(); i++) {
		pathLines.addVertex(pathVertices[i]);
	}
    
    // Update the camera
    cam.updateCam();
    
    ofVec3f camOri = cam.getOrientationEuler();
    printf("Cam Orientation x:%f y:%f z:%f \n",
           camOri.x,
           camOri.y,
           camOri.z);
    ofVec3f curOri = cam.getPosition();
    printf("Cam Position x:%f y:%f z:%f \n",
           curOri.x,
           curOri.y,
           curOri.z);
    
    if(cam.getArrivedAtTarget()){
        printf("Arrived At Target!!! \n");
        sleep(1.0); // Wait for 1 second
    }
}


void testApp::updateCamTarget(){
    // If there are nodes in dequeue, update the nodes to the next target
    if(targetNodes.size()){

        cam.setTarget(targetNodes.front());
        targetNodes.pop_front();
        
        cam.setSnapToTarget(true);
    }
    
}

ofNode testApp::closestTarget(){
    // If there are no waypoints, return null
    if(!targetNodes.size()){
        return curTarNode;
    }
    
    float closestDist = -1;
    ofNode closestNode;
    
    for (deque<ofNode>::iterator it = targetNodes.begin() ; it != targetNodes.end(); ++it){
        ofNode node = *it;
        
        if(closestDist == -1){
            closestDist = (cam.getPosition() - node.getPosition()).length();
            closestNode = node;
            continue;
        }
        
        float distance = (cam.getPosition() - node.getPosition()).length();
        if(distance < closestDist){
            closestDist = distance;
            closestNode = node;
        }
    }
    
    return closestNode;
}

void testApp::toggleCamSnapTarget(){
    cam.setSnapToTarget(!cam.getSnapToTarget());
}

//--------------------------------------------------------------
void testApp::draw(){
	ofColor cyan = ofColor::fromHex(0x00abec);
	ofColor magenta = ofColor::fromHex(0xec008c);
	ofColor yellow = ofColor::fromHex(0xffee00);
	
	ofBackgroundGradient(magenta * .6, magenta * .4);
	ofNoFill();
	
    cam.begin();
	ofRotateX(15);
    
	ofSetColor(0);
	ofDrawGrid(500, 10, false, false, true, false);
    
    ofColor(255,255,255,100);
    ofDrawGrid(500, 10, false, true, false, true);

	
	// draw the path of the box
	ofSetLineWidth(2);
	ofSetColor(cyan);
	pathLines.draw();
	
	// draw a line connecting the box to the grid
	ofSetColor(yellow);
	ofLine(current.x, current.y, current.z, current.x, 0, current.z);
	
	// translate and rotate to the current position and orientation
    ofPushMatrix();
	ofTranslate(current.x, current.y, current.z);
	rotateToNormal(current - previous);
	ofSetColor(255);
	ofBox(32);
	ofDrawAxis(32);
    ofPopMatrix();
    
    for (deque<ofNode>::iterator it = targetNodes.begin() ; it != targetNodes.end(); ++it){
        ofNode node = *it;
        ofSetColor(255, 255, 255);
        node.draw();
    }
    
    // Draw the viewports
    smartViewportManager.drawViewportPreviews();
	
	cam.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key) {
            
        case ' ':{
            // Get new node and set pos + orientation
            ofNode newNode = ofNode();
            newNode.setPosition(current);
            
            float rotationAmount;
            ofVec3f rotationAngle;
            ofQuaternion rotation;
            
            ofVec3f axis(0, 0, 1);
            rotation.makeRotate(axis, current - previous);
            newNode.setOrientation(rotation);
            
            targetNodes.push_back(newNode);
            break;
        }
        
        case OF_KEY_RETURN:{
            if( !isMoving){
                updateCamTarget();
            }
            break;
        }
            
        case 'O':
        case 'o':{
            cam.returnToOrigin();
            break;
        }
            
        case 'X':
        case 'x':{
            float length;
            SmartViewport* closestViewport = smartViewportManager.closestViewport(cam.getPosition(), length);
            
            ofNode viewportPose = closestViewport->getCamPose();
            
            cam.setTarget(viewportPose);
            cam.setSnapToTarget(true);
            break;
        }
            
            
        case 'Z':
        case 'z':{
            cam.setTarget(closestTarget());
            toggleCamSnapTarget();
            break;
        }
            
        case OF_KEY_LEFT: cam.pan(kRotInc); break;
		case OF_KEY_RIGHT: cam.pan(-kRotInc); break;
			
		case OF_KEY_UP: cam.tilt(kRotInc); break;
		case OF_KEY_DOWN: cam.tilt(-kRotInc); break;
			
		case 'q': cam.roll(kRotInc); break;
		case 'e': cam.roll(-kRotInc); break;
			
		case 'a': cam.truck(-kMoveInc); break;
		case 'd': cam.truck(kMoveInc); break;
			
		case 'w': cam.dolly(-kMoveInc); break;
		case 's': cam.dolly(kMoveInc); break;
			
		case 'r': cam.boom(kMoveInc); break;
		case 'f': cam.boom(-kMoveInc); break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}
//--------------------------------------------------------------
// Helper Functionss
//--------------------------------------------------------------

ofQuaternion testApp::quaternionLerp(ofQuaternion start,
                                     ofQuaternion stop,
                                     float pct){
    // Ensure pct between 0 and 1.0
    pct = ofClamp(pct, 0, 1.0);
    ofQuaternion delta = stop - start;
    delta *= pct;
    return start + delta;
}


/*
 rotateToNormal will rotate everything using ofRotate. the rotation amount
 and axis are generated using an ofQuaternion. the trick is to use ofQuaternion
 to determine the rotation that is required from a standard axis (0,0,1) to the
 desired normal vector, then apply that rotation.
 */
//--------------------------------------------------------------
void testApp::rotateToNormal(ofVec3f normal) {
	normal.normalize();
	
	float rotationAmount;
	ofVec3f rotationAngle;
	ofQuaternion rotation;
	
	ofVec3f axis(0, 0, 1);
	rotation.makeRotate(axis, normal);
	rotation.getRotate(rotationAmount, rotationAngle);
	ofRotate(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);
}

/*
 ofQuaternion testApp::OrientationVecToQuat(ofVec3f orientation){
 float rotationAmount;
 ofVec3f rotationAngle;
 ofQuaternion rotation;
 
 ofVec3f axis(0, 0, 1);
 rotation.makeRotate(axis, orientation);
 }
 */