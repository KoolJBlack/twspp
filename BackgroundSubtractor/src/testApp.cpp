#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	camWidth 		= 640;	// try to grab at this size.
	camHeight 		= 480;
	
    vidGrabber.setDeviceID(1);
	vidGrabber.setVerbose(true);
	vidGrabber.initGrabber(camWidth,camHeight);
    
    backgroundSubtractor = BackgroundSubtractor(camWidth, camHeight);
    backgroundSubtractor.zeroBackground();
    
    ROI = ofRectangle(camWidth/4, camHeight/4, camWidth/2, camHeight/2);
    
    backgroundSubtractor.setROI(ROI);
    
    backgroundSubtractor.setImageMirroring(true);
}

//--------------------------------------------------------------
void testApp::update(){
	vidGrabber.update();
    
    ofxCvColorImage camOutput;
    camOutput.setFromPixels(vidGrabber.getPixelsRef());
    backgroundSubtractor.updateImg(camOutput);
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255, 255, 255);
	vidGrabber.draw(0,0);
    
    ofImage bgSubImg = backgroundSubtractor.getBackgroundSubtracktedImage();
    ofImage bgSubImgROI= backgroundSubtractor.getBackgroundSubtracktedImageROI();
    
    ofEnableAlphaBlending();
    bgSubImg.draw(640,0);
    //bgSubImgROI.draw(640,0);
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key){
        case OF_KEY_BACKSPACE:
            backgroundSubtractor.zeroBackground();
            break;
            
        case '=':
            backgroundSubtractor.setThreshold(backgroundSubtractor.getThreshold()+ 1);
            break;

        case '-':
            backgroundSubtractor.setThreshold(backgroundSubtractor.getThreshold() - 1);       
            break;
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

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