//
//  SmartPoly.cpp
//  ShapeTest
//
//  Created by Kojo Acquah on 1/25/13.
//
//

#include "SmartPoly.h"
#include <math.h>


#define SELECTION_NUM_LINES (30)
#define SELECTION_LINE_WIDTH (5)
#define SELECTION_LINE_LENGTH (20)

#define SELECTION_SPEED_MILTIPLIER (100);


#define HIGHLIGHT_COLOR (ofColor(255,255,255))
#define SELECTED_COLOR (ofColor(0,0,255))

SmartPoly::SmartPoly(){
}

SmartPoly::SmartPoly(vector<ofPoint> pts, float r, ofRectangle b) : SmartShape() {
    // Position is derived from bounding ox center
    pos = b.position + ofPoint(b.width/2,b.height/2);
    
    rotation = r;
    boundingBox = b;

    // Setup the poly line
    polyLine = ofPolyline();
    for (vector<ofPoint>::iterator it = pts.begin() ; it != pts.end(); ++it){
        ofPoint p = *it;
        // All points in the poly will be stored respective to pos.
        p = p - pos;
        polyLine.addVertex(p);
    }
    polyLine.close();
        
    // Updat the control points
    updateControlPoints();
}


/*
SmartPoly::SmartPoly(const SmartPoly& other){
    controlPoints = vector<ofPoint>(other.controlPoints.begin(), other.controlPoints.end());
    
    
    printf("Copied \n");
}*/


bool SmartPoly::intersects(SmartPoly &otherPoly){
    // Check to see if any of this poly's pionts lie inside
    // The other poly. 
    for (vector<ofPoint>::iterator it = controlPoints.begin() ; it != controlPoints.end(); ++it){
        const ofPoint p = *it;
        if(otherPoly.containsPoint(p)){
            return true;
        }
    }
    return false;
}


// Inhereted Method

void SmartPoly::draw(){
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(pos.x, pos.y);
    ofRotate(rotation);
    
    
    ofSetColor(color);
    ofNoFill();
    
    ofSetLineWidth(polyLineWidth);
    polyLine.draw();
    
    ofPopStyle();
    ofPopMatrix();
}

void SmartPoly::drawControlPoints(){
    ofPushMatrix();
    ofPushStyle();
    
    // Draw the ancor point as pos
    ofSetColor(controlAnchorColor);
    ofCircle(pos.x, pos.y, controlRadius);

    ofSetColor(controlColor);
    for (vector<ofPoint>::iterator it = controlPoints.begin() ; it != controlPoints.end(); ++it){
        ofPoint p = *it;
        ofCircle(p.x, p.y, controlRadius);
    }
    
    ofPopStyle();
    ofPopMatrix();
}

void SmartPoly::drawShapeImage(){
    // Only draw a shape image if we have one
    if( shapeImageOut.isAllocated()){
        
        switch(interactionMode){
            case SHAPE_INTERACTION_MODE_NORMAL:
            case SHAPE_INTERACTION_MODE_HEIGHTMAP:
            case SHAPE_INTERACTION_MODE_EXTRUDE:

                ofPushMatrix();
                ofPushStyle();
                
                ofTranslate(pos);
                ofRotate(rotation);
                ofSetRectMode(OF_RECTMODE_CENTER);
                // Alpha blending for transparent pixels
                ofEnableAlphaBlending();
                ofColor(255,255,255);
                
                shapeImageOut.draw(0,0);
                
                ofPopStyle();
                ofPopMatrix();
                break;
                
            case SHAPE_INTERACTION_MODE_POPUP:
                ofPushMatrix();
                ofPushStyle();
                
                ofTranslate(pos.x, pos.y);
                ofRotate(rotation);
                
                //Pop out the shape
                //TODO: Determine what rotation is correcte for popup
                ofRotateY(90);
                ofTranslate(0,- boundingBox.height/2);
                
                
                ofSetRectMode(OF_RECTMODE_CENTER);
                // Alpha blending for transparent pixels
                ofEnableAlphaBlending();
                ofColor(255,255,255);
                
                shapeImageOut.draw(0,0);
                
                ofPopStyle();
                ofPopMatrix();
                break;

        }
    }
}

void SmartPoly::drawSelectionMarker(){
    // Don't draw a selection marker if the shape is not selected
    if(selectionMode == SHAPE_SELECTION_MODE_NONE){
        return;
    }
    
    ofPushMatrix();
    ofPushStyle();
    
    ofPoint center(boundingBox.x + boundingBox.width/2,
                   boundingBox.y + boundingBox.height/2);
    ofTranslate(center.x, center.y);
    
    float rotation  = ofGetElapsedTimef() * SELECTION_SPEED_MILTIPLIER;
    int radius = (boundingBox.width + boundingBox.height) / 4;

    ofSetLineWidth(SELECTION_LINE_WIDTH);

    // Color from selection status
    if(selectionMode == SHAPE_SELECTION_MODE_SELECTED){
        ofSetColor(255,0,0);
    }else{
        if(isCopy){
            ofSetColor(0, 0, 255);
        }else{
            ofSetColor(0,255,0);
        }
    }
    
    for( int i = 0; i < SELECTION_NUM_LINES; ++i){
        ofPushMatrix();
        float r = 360.0 / SELECTION_NUM_LINES * i + rotation;
        ofRotate(r);
        
        ofLine(radius,0, radius + SELECTION_LINE_LENGTH, 0);
        ofPopMatrix();
    }
    
    
    ofPopStyle();
    ofPopMatrix();
}


bool SmartPoly::containsPoint(const ofPoint p){
    return ofInsidePoly(p.x, p.y, controlPoints);
    //TODO: this is inaccurate for circles
}

bool SmartPoly::containsPoint(int x, int y){
    return ofInsidePoly(x, y, controlPoints);
    //TODO: this is inaccurate for circles
}

ofRectangle SmartPoly::getBoundingBox(){
    return boundingBox;
}


void SmartPoly::updateControlPoints(){
    // Reset the contorl points vector.
    controlPoints = vector<ofPoint>();

    // Get Polylinepts with pos
    vector<ofPoint> polyLinePrePts = polyLine.getVertices();
    vector<ofPoint> polyLinePts = vector<ofPoint>();
    for (vector<ofPoint>::iterator it = polyLinePrePts.begin() ; it != polyLinePrePts.end(); ++it){
        ofPoint p = *it;
        // All points in the poly will be stored respective to pos.
        p = p + pos;
        polyLinePts.push_back(p);
    }
    
    for (vector<ofPoint>::iterator it = polyLinePts.begin() ; it != polyLinePts.end(); ++it){
        ofPoint p = *it;
        
        // Control point position comes from the pos point and
        // the distance and angle of each point relative to the
        // pos point.
        float length = ofDist(p.x, p.y, pos.x, pos.y);
        float angle = atan2((p.y - pos.y),(p.x - pos.x));
        
        p.x = pos.x + cos(angle + ofDegToRad(rotation)) * length;
        p.y = pos.y + sin(angle + ofDegToRad(rotation)) * length;
        
        controlPoints.push_back(p);
    }
}



