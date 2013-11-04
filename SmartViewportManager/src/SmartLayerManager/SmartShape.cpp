//
//  SmartShape.cpp
//  ShapeTest
//
//  Created by Kojo Acquah on 1/18/13.
//
//

#include "SmartShape.h"

SmartShape::SmartShape(){
    color = ofColor(255,255,255); // White is default polyline color
    meshColor = ofColor(200,200,200, 100); // White is default polyline color

    
    controlColor = ofColor(255,0,0); // Red for control points
    controlAnchorColor = ofColor(0,0,255);  // Blue for the anchor point
    
    pos = ofPoint(0,0,0);
    
    controlRadius = 5;
    
    polyLineWidth = 1;
    
    isCopy = false;
    selectionMode = SHAPE_SELECTION_MODE_NONE;
    interactionMode = SHAPE_INTERACTION_MODE_NORMAL;
}

void SmartShape::setColor(ofColor c) {
    color = c;
}

void SmartShape::setMeshColor(ofColor c) {
    meshColor = c;
}

void SmartShape::setShapeImage(ofImage shapeImage) {
    shapeImageOut = shapeImage;
}

void SmartShape::setPos(ofPoint p) {
    // Get the old bounding box and remove pos
    ofRectangle b = boundingBox;
    b.x -= pos.x;
    b.y -= pos.y;
    
    // Update the pos
    pos = p;
    
    // Update the bounding box
    b.x += pos.x;
    b.y += pos.y;
    boundingBox = b;
    
    updateControlPoints();
    cout<<"called"<<endl;
}

ofPoint SmartShape::getPos(){
    return pos;
}

void SmartShape::setRotation(float r){
    rotation = r;
    
    updateControlPoints();
}

float SmartShape::getRotation(){
    return rotation;
}

void SmartShape::setIsCopy(bool b){
    isCopy = b;
}

bool SmartShape::getIsCopy(){
    return isCopy;
}

void SmartShape::setInteractionMode(ShapeInteractionMode m){
    interactionMode = m;
}

ShapeInteractionMode SmartShape::getInteractionMode(){
    return interactionMode;
}

void SmartShape::setSelectionMode(ShapeSelectionMode m){
    selectionMode = m;
}

ShapeSelectionMode SmartShape::getSelectionMode(){
    return selectionMode;
}


