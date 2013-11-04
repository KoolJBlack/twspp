//
//  SmartPoly.h
//  ShapeTest
//
//  Created by Kojo Acquah on 1/25/13.
//
//

#ifndef ShapeTest_SmartPoly_h
#define ShapeTest_SmartPoly_h

#import "ofMain.h"
#import <vector.h>
#include "SmartShape.h"


class SmartPoly : public SmartShape {
    
protected:
    // Position is represented by the first contorl point
    ofPolyline polyLine;
    ofPath path;
    vector<ofPoint> controlPoints;
    int extrusionHeight;
    
public:
    SmartPoly();
    SmartPoly(vector<ofPoint> pts, float r, ofRectangle b);
    SmartPoly(vector<ofPoint> pts, float r, ofRectangle b, ofPoint cPos);//adding captured point
    // Copy constructor
    //SmartPoly(const SmartPoly& other);
    
    bool intersects(SmartPoly &otherPoly);
    
    // Abstract Methods
    virtual void draw();
    virtual void drawControlPoints();
    
    virtual void drawShapeImage();
    virtual void drawSelectionMarker();
    
    /*CHIGIRA ADDED-----------------------------*/
    virtual void drawMesh();
    virtual void setExtrusionHeight(int height);
    /*CHIGIRA ADDED-----------------------------*/
    
    virtual bool containsPoint(const ofPoint p);
    virtual bool containsPoint(int x, int y);
    
    virtual ofRectangle getBoundingBox();
    
    virtual void updateControlPoints();
    
//    virtual void draw3DObject(float vHeight);
};

#endif
