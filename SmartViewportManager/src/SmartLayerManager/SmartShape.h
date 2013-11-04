//
//  SmartShape.h
//  VidCapture
//
//  Created by Kojo Acquah on 1/18/13.
//
//

#ifndef VidCapture_SmartShape_h
#define VidCapture_SmartShape_h

#import "ofMain.h"

/*enum ShapeType{
 RECTANGLE = 0,
 CIRCLE,
 TRIANGLE
 };*/


enum ShapeSelectionMode{
    SHAPE_SELECTION_MODE_NONE = 0,
    SHAPE_SELECTION_MODE_SELECTED,
    SHAPE_SELECTION_MODE_HIGHLIGHT
};

enum ShapeInteractionMode{
    SHAPE_INTERACTION_MODE_NORMAL = 0,
    SHAPE_INTERACTION_MODE_EXTRUDE,
    SHAPE_INTERACTION_MODE_POPUP,
    SHAPE_INTERACTION_MODE_HEIGHTMAP
};


class SmartShape {
    
protected:
    // Shape data
    ofColor color;
    ofColor meshColor; 
    
    ofColor controlAnchorColor;
    ofColor controlColor;
    int controlRadius;
    
    int polyLineWidth;
    
    // Pose data
    ofPoint pos;
    float rotation;
    
    ofRectangle boundingBox;
    
    // Image data
    ofImage shapeImageOut;
    
    //Shape meta:
    ShapeSelectionMode selectionMode;
    ShapeInteractionMode interactionMode;
    bool isCopy;
    
//    //CHIGIRA ADDED:extrusion height
//    int extrusionHeight;
//    //CHIGIRA ADDED:extrusion height
    
    
public:
    
    SmartShape();
    
    void setColor(ofColor c);
    void setMeshColor(ofColor c);
    void setShapeImage(ofImage shapeImage);
    
    void setPos(ofPoint p);
    ofPoint getPos();
    void setRotation(float r);
    float getRotation();
    
    void setIsCopy(bool b);
    bool getIsCopy();
    
    
    void setInteractionMode(ShapeInteractionMode m);
    ShapeInteractionMode getInteractionMode();
    void setSelectionMode(ShapeSelectionMode m);
    ShapeSelectionMode getSelectionMode();
    
    // Abstract Methods
    virtual void draw() = 0;
    virtual void drawControlPoints() = 0;
    
    virtual void drawShapeImage() = 0;
    // Draws a selection marker around this shape if it is selected
    virtual void drawSelectionMarker() = 0;
    
    virtual void drawMesh() = 0;
    virtual void setExtrusionHeight(int height) = 0;
    
    virtual bool containsPoint(const ofPoint p) = 0;
    virtual bool containsPoint(int x, int y) = 0;
    
    virtual ofRectangle getBoundingBox() = 0;
   
    virtual void updateControlPoints() = 0;
    
    
        ofPoint capturedPos;//ADDED
    
    
};

#endif
