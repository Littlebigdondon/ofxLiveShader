# **ofxLiveShader**

An openFrameworks addon for hot-reloading shader files on the fly. Utilizes Facebook’s Watchman service to monitor filesystem changes. Tested on macOS, but as long as you can build the <a href="#install-dependencies">dependencies</a>, it should work on your platform. 


#### PREREQUISITE NOTES:  

- Using this addon will require you to build OpenFrameworks against a newer version of Boost. That _shouldn’t_ screw up any existing projects (because it looks like the newer Boost headers are almost completely backwards compatible), but you’ve been warned. To make this change, open `OpenFrameworks -> libs -> openFrameworksCompiled -> project -> osx -> CoreOF.xcconfig`, and edit `HEADER_BOOST` to point to `"/usr/local/include"` (as opposed to `"$(OF_PATH)/libs/boost/include"`).
- On macOS, it seems the Watchman dependency will only build for targets of macOS 10.13 and up. Any project wishing to use this addon will need to (in Xcode) navigate to `Project -> General -> Deployment Info` and change `Deployment Target` to at least `10.13`.



## Using this Addon 

_**Note**: This currently assumes you’re using Xcode. If you got this built & running on another platform, and want to document it, reach out and I’ll update this guide._


### Install Dependencies 

- Install [Watchman](https://facebook.github.io/watchman/docs/install.html) and its dependencies (namely, you’ll need [Folly](https://github.com/facebook/folly) and _its_ dependencies). On macOS with Homebrew, this should be something along the lines of
```brew install folly watchman```
	

### Project Setup 

- Simply include this as an addon when using the Project Generator, or, if adding manually, be sure to include each source file in `Project -> [select target] -> Build Phases -> Compile Sources` 
- Add `libfolly.dylib,` `libevent.dylib`, `libglog.dylib,` and `libssl.dylib` to `Project -> [select target] -> General -> Linked Frameworks and Libraries`.
_**NOTE**: After clicking “`+`” to add a framework, I had to manually select “`Add Other`”, and navigate to `/usr/local/opt` (using `cmd+shift+G` should help out) to get to these libraries._
- Add `/usr/local/include` to `Project -> [select target] -> Build Settings -> Header Search Paths`

_**NOTE**: This next step may not be necessary based on your project/dependency settings/how far into the future you’re doing this, but as of right now, Xcode defaults to c++11 so you absolutely must change it to build against c++14 at minimum (the _`-std=c++1z`_ flag below will build against c++17, for example)._

- Add a few C++ compiler flags to your target under `Project -> [select target] -> Build Settings -> Other C++ Flags` :
    ```
    -I/usr/local/opt/openssl/include
    -std=c++1z
    ```


### Usage 

- First, in `ofApp.h`, we need to add `#include "ShaderFileWatcher.h"` to the list of includes, and create a public ShaderFileWatcher pointer member in the `ofApp` class
    ```c++
    class ofApp : public ofBaseApp {
      public:
        ...
        ...
        ShaderFileWatcher* fileWatcher;
        // And obviously you'll be updating a shader:
        ofShader shader;
    }
    ```

- Next, we must instantiate the `folly` library in our main. The `init `function requires `argc` and `argv`, so we’ll need to expose these parameters in `main.cpp` by changing 
    ```c++
    int main() {…}
    ``` 
    to 
    ```c++
    int main(int argc, char** argv) {…}
    ```
    and immediately passing these to `folly::init()`. I'm going to only pass the first argument because of [this](https://stackoverflow.com/questions/46103109/xcode-and-python-error-unrecognized-arguments-nsdocumentrevisionsdebugmode).
    ```c++
    int main(int argc, char** argv) {
      argc = 1;
      folly::init(&argc, &argv);
      ...
    }
    ```

- Awesome! With all of that out of the way, now we can instantiate a `fileWatcher` which will tell the `watchman` server to watch a directory of our choosing, and will automatically listen for changes to certain (or any) shader files within that directory. So navigate to `ofApp.cpp`, and in `setup()`, initialize the ShaderFileWatcher pointer
    ```c++
    void ofApp::setup() {
      fileWatcher = new ShaderFileWatcher(
                                // 1. Folder containing shader files (default = project/bin/data)
                              "/absolute/path/to/folder/of/shaders",
                                // 2. Name of vertex shader to monitor for changes (default = "*.vert")
                              "shader.vert",
                                // 3. Name of fragment shader to monitor for changes (default = "*.frag")
                              "shader.frag",
                                // 4. Path to your Watchman executable (default = "/usr/local/bin")
                              "/usr/local/bin"
                            );
      ...
      ...
    }
    ```

- Lastly, all we have to do is call `checkForUpdate()`, which will return `true` if we need to reload our shaders

    ```c++
    void ofApp::update() {
      if (fileWatcher->checkForUpdate()) {
        shader.load("shader.vert", "shader.frag");
      }
      ...
      ...
    }
    ```

- That’s it! You can now compile openFrameworks once, and write your shaders on the fly (even in a different editor!). Check the example project to see it all put together. Happy shading!


