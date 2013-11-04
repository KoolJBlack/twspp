#include "testApp.h"
#include <vector.h>

#define vidWidth (800)
#define vidHeight (600)

//--------------------------------------------------------------
void testApp::setup(){    
    vidGrabber.setDeviceID(0);
    vidGrabber.initGrabber(vidWidth,vidHeight);
    
    //smartLayerManager = SmartLayerManager();
    smartLayerManager.initWithGrabber(&vidGrabber);
    smartLayerManager.setSmartLayerScale(0.5);
    
    layerOffset = 0;
    selected = false;
}

//--------------------------------------------------------------
void testApp::update(){
    smartLayerManager.update();
    
    // Don't do anything if we already have a selected shape
    if(selected){
        return;
    }
    
    // Get the selected shape from the bottom right screen
    int x = mouseX * 2;
    int y = (mouseY - 600) * 2;
    
    // Release the old shape
    if(selectedShape){
        selectedShape->setSelectionMode(SHAPE_SELECTION_MODE_NONE);
    }
    
    // Get the new shape
    selectedShape = smartLayerManager.onSelectShape(x, y);
    
    // Set its selection mode
    if(selectedShape){
        selectedShape->setSelectionMode(SHAPE_SELECTION_MODE_HIGHLIGHT);
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    // Draw the live video image
    smartLayerManager.drawVidGrabber(0,0);
    // Draw any new contours detected ontop of the live video image
    smartLayerManager.drawContours(0,0);
    
    // Draw the gray diff threshold. This is used for contour finding
    smartLayerManager.drawGrayDiffThreshold(800,0);
    
    
    // Draw White background for each laer and rectangle for layer placeholders
    ofPushStyle();
    ofSetColor(220, 220, 220);
    ofRect(0, vidHeight, ofGetWindowWidth(), (ofGetWindowHeight() - vidHeight));
    ofSetColor(0, 0, 0);
    ofNoFill();
    ofSetLineWidth(3);
    for( int i = 0; i < 4; ++i){
        ofRect(i * vidWidth/2, vidHeight, vidWidth/2, vidHeight/2);
    }
    ofPopStyle();
    
    
    // Draw the last 4 smart layers, if they exist
    int numLayers = smartLayerManager.getNumSmartLayers();
    
    layerOffset = ofClamp(layerOffset, 0, ( 0 > numLayers - 4) ? 0 : (numLayers - 4));

    for (int i = 0; i < numLayers && i < 4; ++i){
        smartLayerManager.drawLayerSmartShapes(numLayers - (i + layerOffset) - 1 , (vidWidth * i) / 2, vidHeight);
        smartLayerManager.drawLayerSmartShapeOverlays(numLayers - (i + layerOffset) - 1 , (vidWidth * i) / 2, vidHeight);

    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == ' '){
        smartLayerManager.layerSnapShot();
    }else if(key == OF_KEY_BACKSPACE){
        smartLayerManager.zeroBackground();
    }else if(key == 'c' || key == 'C'){
        smartLayerManager.resetLayers();
    }else if(key == OF_KEY_UP){
        smartLayerManager.layerSelectUp();
    }else if(key == OF_KEY_DOWN){
        smartLayerManager.layerSelectDown();
    }else if(key == 'd' || key == 'D'){
        if(selectedShape && selected){
            // Release the current shape
            selectedShape->setSelectionMode(SHAPE_SELECTION_MODE_NONE);
            
            // Get a copy of the current shape
            selectedShape = smartLayerManager.duplicateShapeInLayer((SmartPoly*)selectedShape, smartLayerManager.getSelectedLayerNum());
            
            // Select the copy
            selectedShape->setSelectionMode(SHAPE_SELECTION_MODE_SELECTED);
        }
    }
    
    if(selected){
        float r = selectedShape->getRotation();

        if(key == OF_KEY_LEFT){
            selectedShape->setRotation(r-=5);
        }else if(key == OF_KEY_RIGHT){
            selectedShape->setRotation(r+=5);
        }else if(key == 'k' || key == 'K'){
            smartLayerManager.removeCopyFromLayer(selectedShape, smartLayerManager.getSelectedLayerNum());
            
            // They layer manager only removes the shape, it does not delte
            // it, so we must delete the shape ourselves.
            delete (SmartPoly*)selectedShape;
            
            selected = false;
            selectedShape = NULL;
            
        }else if(key == '=' || key == '+'){
            // Move the shape up 10 units
            ofPoint pos = selectedShape->getPos();
            pos.z += 10;
            selectedShape->setPos(pos);
        }else if(key == '-' || key == '_'){
            // Move the shape down 10 units
            ofPoint pos = selectedShape->getPos();
            pos.z -= 10;
            selectedShape->setPos(pos);
        }else if(key == '9' || key == '('){
            // Change the current slected shapes interaction mode to normal
            selectedShape->setInteractionMode(SHAPE_INTERACTION_MODE_NORMAL);
        }else if(key == '0' || key == ')'){
            // Change the current slected shapes interaction mode to popup
            // The shape will now stand up
            selectedShape->setInteractionMode(SHAPE_INTERACTION_MODE_POPUP);
        }
    }else{
        if(key == OF_KEY_LEFT){
            // Shift layers displayed in the bottom to the right
            layerOffset -= 1;
        }else if(key == OF_KEY_RIGHT){
            // Shift layers displayed in the bottom to the left
            layerOffset += 1;
        }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){    
    // If mouse is moved with a selected shape, update its pos
    if(selectedShape){
        if(selected){
            ofPoint pos = selectedShape->getPos();
            
            pos.x = (mouseX) * 2;
            pos.y = (mouseY - 600) * 2;
            
            selectedShape->setPos(pos);
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    // If we are hovering over a shape, capture it. 
    if(selectedShape){
        if(selected){
            selectedShape->setSelectionMode(SHAPE_SELECTION_MODE_HIGHLIGHT);
            selected = !selected;
        }else{
            selectedShape->setSelectionMode(SHAPE_SELECTION_MODE_SELECTED);
            selected = !selected;
        }
    }

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