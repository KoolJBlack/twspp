//
//  SmartLayer.cpp
//  SmartLayerCapture
//
//  Created by Kojo Acquah on 1/25/13.
//
//

#include "SmartLayer.h"

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
        s->draw();
        s->drawControlPoints();
    }
    
    ofPopMatrix();
}

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
        
        if (s->containsPoint(x, y)){
            return s;
        }
    }
    
    // No shape was selected, so return null
    return NULL;
}







