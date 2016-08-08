#include "ofApp.h"

//Played at BSoD 2016


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(false);
    ofBackground(0);
    
    fbo.allocate(ofGetWidth(), ofGetHeight());
    postEffect.setup(&fbo);
    
    
    camSwitch = true;
    
    cam.setFov(80);
    cam.setNearClip(0);
    cam.setFarClip(99999);
    
    cam.setFov(80);
    cam2.setNearClip(0);
    cam2.setFarClip(35000);
    
    
    //Astronaut
    astronaut.loadModel("3dmodel/astronaut3.dae", 20);
    astronaut.setPosition(0, 60, 0);
    universe.set(27.5, 30);
    
    
    //Earth
    //
    //Shaderに渡す画像は、ロードする直前にofDisableArbTexを行う必要があるよ！
    ofDisableArbTex();
    envImage.load("tex/milkyway.jpg");
    shader.load("Shaders/equirectEnvMap");
    
    ofEnableArbTex();
    ofLoadImage(tex, "tex/earth.jpg");
    pfbo.allocate(2000, 1000);
    earth.mapTexCoords(0, pfbo.getHeight(), pfbo.getWidth(), 0);
    
    
    //Terrain
    //
    tcolor = ofFloatColor(0.0, 0.5, 0.5);
    terrain.setup(8000, 8000, &tcolor);
    
    tcolor2 = ofFloatColor(1.0, 0.5, 0.5);
    terrain2.setup(8000, 8000, &tcolor2);
    
    
    //Guis-------
    //
    gui.setup();
    gui.add(speed.setup("speed", 0.0, 0, 1.0));
    gui.add(height.setup("height", 600, 0, 6000.0));
    gui.add(fov.setup("fov", 60.0, 30.0, 160.0));
    gui.add(terrain_pos.setup("terrain_pos", 50000, 2750, 50000));
    gui.add(earth_size.setup("earth_size", 5.0, 5.0, 500.0));
    gui.add(univ_size.setup("univ_size", 27.5, 27.5, 2700.0));
    gui.add(astro_size.setup("astro_size;", 0.8, 0.0, 0.8));
    
    
    //Syphon------
    //
    mainOutputSyphonServer.setName("Screen Output");
    individualTextureSyphonServer.setName("Texture Output");
    mClient.setup();
    
    //using Syphon app Simple Server, found at http://syphon.v002.info/
    mClient.set("","Simple Server");
}

//--------------------------------------------------------------
void ofApp::update(){
    time = ofGetElapsedTimef();
    
    float terrain_pos = tween_terrain.update();
    float earth_radius = tween_earth.update();
    float univ_radius = tween_univ.update();
    float astro_size = tween_astro.update();
    
    
    cam.setFov(fov);
    cam.lookAt(universe.getPosition());
    cam2.lookAt(ofVec3f(0));
    cam2.setPosition(400 * sin(time * 0.5), 400 * cos(time * 0.5), 400 * cos(time * 0.5));
    
    
    astronaut.setScale(astro_size, astro_size, astro_size);
    
    earth.set(earth_radius, 20);
    universe.set(univ_radius, 20);
    
    terrain.update(speed, height, terrain_pos);
    terrain2.update(speed, height, -terrain_pos);
    
    
    pfbo.begin();
    ofClear(0, 0, 0, 255);
    tex.draw(0, 0, 2000, 1000);
    pfbo.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //よくわかんないけど、Sypthonのexampleはdrawの最初にglClearColor()とglClear()を呼んでるから入れてる。
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);
    
    fbo.begin();
    ofClear(0, 0, 0, 255);
    
    
    if(camSwitch) cam.begin();
    if(!camSwitch) cam2.begin();
    
    
    //astronaut
    //
    glPushMatrix();
    ofTranslate(0, -14, 3);
    
    ofTranslate(300*ofNoise(time * 0.4), -300*ofNoise(time * 0.4+0.1), 300*ofNoise(time * 0.2+0.2));
    ofRotateX(time*15);
    ofRotateY(time*20);
    ofRotateZ(time*10);
    astronaut.drawFaces();
    
    
    //universe
    //
    ofTranslate(1, -315, 27.5);
    shader.begin();
    shader.setUniformTexture("envMap", envImage.getTexture(), 0);
    universe.draw();
    shader.end();
    
    
    
    //earth
    //
    ofRotateX(time * -10);
    ofRotateY(time * -5);
    
    pfbo.getTexture().bind();
    earth.draw();
    pfbo.getTexture().unbind();
    
    glPopMatrix();
    
    
    //terrain
    //
    terrain.drawWireframe();
    terrain2.drawWireframe();
    
    
    if(!camSwitch) cam.end();
    if(camSwitch) cam2.end();
    
    glDisable(GL_DEPTH_TEST);
    fbo.end();
    
    
    postEffect.generateFx();
    
    fbo.draw(0, 0);
    
    gui.draw();
    ofDrawBitmapString(ofToString(ofGetFrameRate()), ofPoint(20, 20));
    
    
    mainOutputSyphonServer.publishScreen();
    
}

//--------------------------------------------------------------
void ofApp::exit() {
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    unsigned delay = 0;
    unsigned duration = 700;
    
    if(key == ' ') camSwitch = !camSwitch;
    
    //地球シーン
    //
    if(key == 'e'){
        tween_terrain.setParameters(4, easing_terrain, ofxTween::easeOut, 500, 50000, duration, delay);
        tween_earth.setParameters(4, easing_earth, ofxTween::easeOut, 5, 500, duration, delay);
        tween_univ.setParameters(4, easing_univ, ofxTween::easeOut, 27.5, 2700, duration, delay);
        tween_astro.setParameters(4, easing_astro, ofxTween::easeOut, 0.8, 0, duration, delay);
    }
    
    //地形シーン
    //
    if(key == 'c'){
        tween_terrain.setParameters(4, easing_terrain, ofxTween::easeOut, 50000, 500, duration, delay);
        tween_earth.setParameters(4, easing_earth, ofxTween::easeOut, 500, 5, duration, delay);
        tween_univ.setParameters(4, easing_univ, ofxTween::easeOut, 2700, 27.5, duration, delay);
        tween_astro.setParameters(4, easing_astro, ofxTween::easeOut, 0, 0.8, duration, delay);
    }
    
    
    //PostEffects
    //
    if(key == '1') postEffect.setFx(OFXPOSTGLITCH_CONVERGENCE , true);
    if(key == '2') postEffect.setFx(OFXPOSTGLITCH_CUTSLIDER   , true);
    if(key == '3') postEffect.setFx(OFXPOSTGLITCH_KAREIDOSCOPE, true);
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == '1') postEffect.setFx(OFXPOSTGLITCH_CONVERGENCE , false);
    if(key == '2') postEffect.setFx(OFXPOSTGLITCH_CUTSLIDER   , false);
    if(key == '3') postEffect.setFx(OFXPOSTGLITCH_KAREIDOSCOPE, false);
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
