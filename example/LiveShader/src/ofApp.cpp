#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    fileWatcher = new ShaderFileWatcher("/Users/Donnie/Documents/OpenFrameworks/of_v0.10.1_osx_release/addons/ofxLiveShader/example/LiveShader/bin/data",
                                     "",
                                     "shader.frag");
}

//--------------------------------------------------------------
void ofApp::update(){
    if (fileWatcher->checkForUpdate()) {
        shader.load("", "shader.frag");
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
//    if (file)
    int windowWidth = ofGetWindowWidth();
    int windowHeight = ofGetWindowHeight();
    
    shader.begin();
    shader.setUniform2f("iResolution", windowWidth, windowHeight);
    shader.setUniform1f("iTime", ofGetElapsedTimef());
    ofDrawPlane(windowWidth/2, windowHeight/2, windowWidth, windowHeight);
    shader.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
