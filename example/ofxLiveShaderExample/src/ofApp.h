#pragma once

#include "ofMain.h"

// !!__NECESSARY__!!
#include "ShaderFileWatcher.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        // !!__NECESSARY__!!
        // Well we're obviously going to need a shader to draw...
        // and a pointer to the file watching service we'll define later.
        ofShader shader;
        ShaderFileWatcher* fileWatcher;
		
};
