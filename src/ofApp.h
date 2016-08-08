#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxGui.h"
#include "ofxPostGlitch.h" //Added Kareidoscope Effect
#include "ofxSyphon.h"
#include "ofxTween.h"

#include "Terrain.hpp"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofEasyCam cam, cam2;
    bool camSwitch;
    float time;
    
    ofxPostGlitch postEffect;
    ofFbo fbo;
    
    //Objects-------
    ofxAssimpModelLoader astronaut;
    
    ofSpherePrimitive universe;
    ofImage envImage;
    ofShader shader;
    
    ofSpherePrimitive earth;
    ofTexture tex;
    ofFbo pfbo;
    
    ofFloatColor tcolor, tcolor2;
    Terrain terrain, terrain2;
    //Objects-------
    
    //easing
    //
    ofxTween tween_terrain;
    ofxTween tween_earth;
    ofxTween tween_univ;
    ofxTween tween_astro;
    ofxEasingCubic easing_terrain;
    ofxEasingCubic easing_earth;
    ofxEasingCubic easing_univ;
    ofxEasingCubic easing_astro;
    
    
    //Guis
    //
    ofxFloatSlider speed;
    ofxFloatSlider fov;
    ofxFloatSlider height;
    ofxFloatSlider  terrain_pos;
    ofxFloatSlider  earth_size;
    ofxFloatSlider  univ_size;
    ofxFloatSlider  astro_size;
    ofxPanel gui;
    
    
    //Syphon
    //
    ofxSyphonServer mainOutputSyphonServer;
    ofxSyphonServer individualTextureSyphonServer;
    
    ofxSyphonClient mClient;
};
