//
//  SmartLayer.cpp
//  SmartLayerCapture
//
//  Created by Kojo Acquah on 1/25/13.
//
//

#include "SmartLayer.h"

#define DETECTIONTH (150)

SmartLayer::SmartLayer(){
    
}

SmartLayer::SmartLayer(ofxCvColorImage i, ofxCvGrayscaleImage g, vector<SmartShape*> s){
    layerImage = i;
    grayDiffImage = g;
    smartShapes = s;
    
    // Dilate grayDiff for drawing later
    grayDiffImage.dilate();
    
    initShapeImages();
}

void SmartLayer::initShapeImages(){
    for (vector<SmartShape*>::iterator it = smartShapes.begin() ; it != smartShapes.end(); ++it){
        // Retrieve each shape's bounding box
        SmartShape* s = *it;
        ofRectangle b = s->getBoundingBox();
        
        // Get ROI pixels of images based on shape's bounding boxes
        grayDiffImage.setROI(b.x, b.y, b.width, b.height);
        unsigned char * diffPixels = grayDiffImage.getRoiPixels();
        layerImage.setROI(b.x, b.y, b.width, b.height);
        unsigned char * colorPixels = layerImage.getRoiPixels();
        
        int bx = b.x;
        int by = b.y;
        int bw = b.width;
        int bh = b.height;
        
        int numPixels = b.width * b.height;
        
        // Color pixels with transparency for shape drawing images
        unsigned char * shapePixels = new unsigned char[numPixels * 4];
        
        for(int i = 0; i < bw; ++i){
            for(int j = 0; j < bh; ++j){
                // Set the color pixels
                int index = (bw * j + i);
                shapePixels[index*4]=colorPixels[index*3];
                shapePixels[index*4+1]=colorPixels[index*3+1];
                shapePixels[index*4+2]=colorPixels[index*3+2];
                
                // If threshold pixel is white, set pixel to opaque
                if(diffPixels[index] > 200){
                    shapePixels[index*4+3] = 255;
                }else{
                    shapePixels[index*4+3] = 0;
                }
            }
        }
        
        // Set the shape
        ofImage i = ofImage();
        i.setFromPixels(shapePixels, bw, bh, OF_IMAGE_COLOR_ALPHA);
        s->setShapeImage(i);
        
    }// end for vector
}

int SmartLayer::getNumSmartShapes(){
    return smartShapes.size();
}

void SmartLayer::addSmartShape(SmartShape *newShape){
    smartShapes.push_back(newShape);
}

void SmartLayer::removeSmartShape(SmartShape*  shape){
    for (vector<SmartShape*>::iterator it = smartShapes.begin() ; it !=
         smartShapes.end(); ++it){
        SmartShape* s = *it;
        if(s == shape){
            smartShapes.erase(it);
            printf("Erase success!\n");
            return;
        }
        
    }
    
    throw new string("Could not find shape in layer to remove");
}


vector<SmartShape*> SmartLayer::getSmartShapes(){
    return smartShapes;
}

ofxCvColorImage SmartLayer::getLayerImage(){
    return layerImage;
}

ofxCvGrayscaleImage SmartLayer::getGrayDiffImage(){
    return grayDiffImage;
}

void SmartLayer::drawSmartShapes(int x, int y, float scale){
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(x,y);
    ofScale(scale, scale);
    
    for (vector<SmartShape*>::iterator it = smartShapes.begin() ; it !=
         smartShapes.end(); ++it){
        SmartShape* s = *it;
        
        s->drawShapeImage();
        s->drawSelectionMarker();
        
    }
    ofPopStyle();
    ofPopMatrix();
}

void SmartLayer::drawSmartShapeOverlays(int x, int y, float scale){
    ofPushMatrix();
    ofTranslate(x,y);
    ofScale(scale, scale);
    
    // Draw each SmartShape's outline and contorl points
    for (vector<SmartShape*>::iterator it = smartShapes.begin() ; it != smartShapes.end(); ++it){
        SmartShape* s = *it;
        ofPushMatrix();
        ofTranslate(s->capturedPos.x, s->capturedPos.y);
        s->draw();
        ofPopMatrix();
    }
    
    ofPopMatrix();
}

/*CHIGIRA ADDED-----------------------------------*/

void SmartLayer::drawSmartMesh(int x, int y, float scale) {
    ofPushMatrix();
    ofTranslate(x,y);
    ofScale(scale, scale);
    //ofColor color(200, 200, 200, 150);
    for (vector<SmartShape*>::iterator it = smartShapes.begin() ; it != smartShapes.end(); ++it){
        SmartShape* s = *it;
        ofPushMatrix();
        ofTranslate(s->capturedPos.x, s->capturedPos.y);
        s->drawMesh();
        ofPopMatrix();
    }
    ofPopMatrix();
}
/*
void SmartLayer::drawSmartMesh(int x, int y, float scale, int depth, ofPoint mkPos, bool bChangeExtrusionHeight, ofColor color) {
    ofPushMatrix();
    ofTranslate(x,y);
    ofScale(scale, scale);
    ofColor dstColor;

    // Draw each SmartShape's outline and contorl points
    for (vector<SmartShape*>::iterator it = smartShapes.begin() ; it != smartShapes.end(); ++it){
        SmartShape* s = *it;
        ofPoint pp = s->getPos();
        if((mkPos.x- pp.x)*(mkPos.x- pp.x) + (mkPos.y - pp.y)*(mkPos.y- pp.y)<DETECTIONTH) {
            dstColor = color;
            if(bChangeExtrusionHeight) s->setExtrusionHeight(depth);
        }else {
            dstColor.set(200, 200, 200, 150);
        }
        s->drawMesh(dstColor);
    }
    ofPopMatrix();
}




void SmartLayer::drawSmartMesh(int x, int y, float scale, int depth[2], ofPoint mkPos[2], bool bChangeExtrusionHeight[2], ofColor color[2]) {
    ofPushMatrix();
    ofTranslate(x,y);
    ofScale(scale, scale);
    
    float dstDis;
    ofColor dstColor;
    int dstDepth;
    
    // Draw each SmartShape's outline and contorl points
    for (vector<SmartShape*>::iterator it = smartShapes.begin() ; it != smartShapes.end(); ++it){
        SmartShape* s = *it;
        ofPoint pp = s->getPos();
        float dis1 = (mkPos[0].x- pp.x)*(mkPos[0].x- pp.x) + (mkPos[0].y - pp.y)*(mkPos[0].y- pp.y);
        float dis2 = (mkPos[1].x- pp.x)*(mkPos[1].x- pp.x) + (mkPos[1].y - pp.y)*(mkPos[1].y- pp.y);
        if(dis1<dis2) {
            dstDis = dis1;
            dstColor = color[0];
            dstDepth = depth[0];
        } else {
            dstDis = dis2;
            dstColor = color[1];
            dstDepth = depth[1];
        }
        if(dstDis<DETECTIONTH) {
            if(bChangeExtrusionHeight[0]) s->setExtrusionHeight(dstDepth);
        }else {
            dstColor.set(200, 200, 200, 150);
        }
        s->drawMesh(dstColor);
    }
    ofPopMatrix();
}


void SmartLayer::drawSmartMesh(int x, int y, float scale, ofVec3f mkInfo[2], bool bChangeExtrusionHeight[2], ofColor color[2]) {
    ofPushMatrix();
    ofTranslate(x,y);
    ofScale(scale, scale);
    
    float dstDis;
    ofColor dstColor;
    int dstDepth;
    
    // Draw each SmartShape's outline and contorl points
    for (vector<SmartShape*>::iterator it = smartShapes.begin() ; it != smartShapes.end(); ++it){
        SmartShape* s = *it;
        ofPoint pp = s->getPos();
        int id;
        float dis1 = (mkInfo[0].x- pp.x)*(mkInfo[0].x- pp.x) + (mkInfo[0].y - pp.y)*(mkInfo[0].y- pp.y);
        float dis2 = (mkInfo[1].x- pp.x)*(mkInfo[1].x- pp.x) + (mkInfo[1].y - pp.y)*(mkInfo[1].y- pp.y);
        if(dis1<dis2) {
            dstDis = dis1;
            dstColor = color[0];
            dstDepth = mkInfo[0].z;
            id = 0;
        } else {
            dstDis = dis2;
            dstColor = color[1];
            dstDepth = mkInfo[1].z;
            id = 1;
        }
        dstColor.a = 40;
        if(dstDis<DETECTIONTH) {
            if(bChangeExtrusionHeight[id]) s->setExtrusionHeight(dstDepth);
        }else {
            dstColor.set(200, 200, 200, 50);
        }
        s->drawMesh(dstColor);
    }
    ofPopMatrix();
}*/

/*CHIGIRA ADDED-----------------------------------*/

void SmartLayer::drawLayerImage(int x, int y, float scale){
    ofPushMatrix();
    ofTranslate(x,y);
    ofScale(scale, scale);
    
    layerImage.draw(0,0);
    
    ofPopMatrix();
}


SmartShape * SmartLayer::onShapeSelect(int x, int y){
    for (vector<SmartShape*>::iterator it = smartShapes.begin() ; it !=
         smartShapes.end(); ++it){
        // Retrieve each shape's bounding box
        SmartShape* s = *it;
        
//        if (s->containsPoint(x, y)){
        if (s->containsPoint(x - s->capturedPos.x, y - s->capturedPos.y)){
            return s;
        }
    }
    
    // No shape was selected, so return null
    return NULL;
}







