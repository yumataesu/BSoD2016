//
//  Terrain.hpp
//  BsoD2016
//
//  Created by Yuma Taesu on 2016/08/08.
//
//

#ifndef Terrain_hpp
#define Terrain_hpp

#include "ofMain.h"

class Terrain {
    
public:
    void setup(int w_, int h_, ofFloatColor* color_);
    void update(float speed_, float terrain_height_, float vertical_livel_);
    void drawWireframe();
    
    
private:
    ofVboMesh vbomesh;
    ofFloatColor* color;
    
    int cols, rows;
    int w, h;
    const int scl = 100;
    float speed, terrain_height, vertical_livel;
    float flying;
    
};

#endif /* Terrain_hpp */
