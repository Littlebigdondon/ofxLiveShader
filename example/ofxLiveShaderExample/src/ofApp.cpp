#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    /**
    /// Define our ShaderFileWatcher (delclared in "ofApp.h")
    /// Params:
    ///      1. string - Absolute path to folder containing your shader files (default = project/bin/data).
    ///      2. string - Name of vertex shader file (default = "*.vert",
    ///                      but you can pass "" to ignore, or an actual filename for exact match).
    ///      3. string - Name of fragment shader file (default = "*.frag",
    ///                      but you can pass "" to ignore, or an actual filename for exact match).
    ///      4. string - Path to watchman executable (default = "/usr/local/bin").
    ///                      Check ShaderFileWatcher.hpp for notes.
    **/
    /**
    ///    If, for example, you don't need a vertex shader,
    ///    and you only want to trigger updates when "shader_for_open_frameworks.frag"
    ///    gets modified, you could do something like this instead:
    ///
    ///  fileWatcher = new ShaderFileWatcher("/absolute/path/to/a/directory/containing/shader/files",
    ///                                      "",
    ///                                      "shader_for_open_frameworks.frag");
    **/
    fileWatcher = new ShaderFileWatcher();
}

//--------------------------------------------------------------
void ofApp::update(){
    /**
    /// Call this (it's the ONLY function available in the ShaderFileWatcher class)
    /// NOTE: this will be true when the watched file(s) have changed
    ///          (aka reload your f@#KING shaders dude!!)
    **/
    if (fileWatcher->checkForUpdate()) {
        shader.load("", "shader.frag");
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Do your normal draw loop stuff. Nothing magical here.
    int windowWidth = ofGetWindowWidth();
    int windowHeight = ofGetWindowHeight();
    
    shader.begin();
    shader.setUniform2f("iResolution", windowWidth, windowHeight);
    shader.setUniform1f("iTime", ofGetElapsedTimef());
    ofDrawPlane(windowWidth/2, windowHeight/2, windowWidth, windowHeight);
    shader.end();
}
