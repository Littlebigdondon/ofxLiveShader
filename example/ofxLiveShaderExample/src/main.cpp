#include "ofMain.h"
#include "ofApp.h"

//========================================================================
// Unfortunately we have to capture these two arguments (argc & argv) to pass to folly.
int main(int argc, char** argv){
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

    // !!__NECESSARY__!!
    // Initialize Folly
    //
    // NOTE: I really don't want to have to do this...I'm not sure of any
    //   simpler way to capture these necessary args & start Folly. :(
    //   Ideally this would be done inside the ShaderFileWatcher class.
    argc = 1;
    folly::init(&argc, &argv);
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
