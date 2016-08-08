//
//  Terrain.cpp
//  BsoD2016
//
//  Created by Yuma Taesu on 2016/08/08.
//
//

#include "Terrain.hpp"

void Terrain::setup(int w_, int h_, ofFloatColor* color_) {
    vbomesh.clear();
    h = h_;
    w = w_;
    
    rows = w_ / scl;
    cols = h_ / scl;
    color = color_;
    
    vbomesh.setMode(OF_PRIMITIVE_TRIANGLES);
    for(int y = 0; y < rows; y++){
        for(int x = 0; x < cols; x++){
            vbomesh.addVertex(ofVec3f(x * scl, y * scl, 0));
            vbomesh.addColor(ofFloatColor(*color));
        }
    }
    
    for(int j = 0; j < rows-1; j++){
        for(int i = 0; i < cols-1; i++){
            vbomesh.addTriangle(i+(j*cols), (i+(j*cols))+cols, (i+j*cols)+1);
        }
    }
}

void Terrain::update(float speed_, float terrain_height_, float vertical_livel_){
    
    speed = speed_;
    terrain_height = terrain_height_;
    vertical_livel = vertical_livel_;
    
    flying += speed;
    
    float yoff = flying;
    for(int j = 0; j < rows; j++){
        float xoff = 0;
        for(int i = 0; i < cols; i++){
            ofVec3f verts = vbomesh.getVertex(i+(j*cols));
            float z = ofMap(ofSignedNoise(xoff, yoff), 0, 1, -100, terrain_height);
            
            vbomesh.setVertex(i+(j*cols), ofVec3f(verts.x, verts.y, z+vertical_livel));
            vbomesh.setColor(i+(j*cols), ofFloatColor(*color));
            xoff += 0.2;
        }
        yoff += 0.2;
    }
}

void Terrain::drawWireframe(){
    glPushMatrix();
    ofTranslate(-w/2, -h/2);
    vbomesh.drawWireframe();
    glPopMatrix();
}
