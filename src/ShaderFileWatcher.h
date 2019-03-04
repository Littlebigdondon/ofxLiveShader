//
//  ShaderFileWatcher.hpp
//  LiveShader
//
//  Created by Donnie Pitts on 3/1/19.
//

#ifndef ShaderFileWatcher_h
#define ShaderFileWatcher_h

#include "WatchmanClient.h"
#include "ofMain.h"

#include <stdio.h>
#include <folly/init/Init.h>
#include <folly/io/async/EventBaseThread.h>
#include <glog/logging.h>

using namespace std;
using namespace folly;
using namespace watchman;


class ShaderFileWatcher {
public:
    string shaderPath;
    string vertexShader;
    string fragmentShader;
    string executablesPath;
    
    ShaderFileWatcher(string shaderPath = std::filesystem::current_path().string() + "/../../../data",
                      string vertexShader = "*.vert",
                      string fragmentShader = "*.frag",
                      string executablesPath = "/usr/local/bin/");
    ~ShaderFileWatcher();
    
    bool checkForUpdate();
    
private:
    string subscriptionName = "ShaderFileWatcher";
    dynamic query = dynamic::object("fields", dynamic::array("name"))(
                                      "expression",
                                          dynamic::array("anyof",
                                             dynamic::array("match", this->vertexShader),
                                             dynamic::array("match", this->fragmentShader)));
    shared_ptr<WatchmanClient> client;
    shared_ptr<EventBaseThread> eventBaseThread;
    const ErrorCallback errorCallback =
        ErrorCallback([](const exception_wrapper& ex) {
                   cerr << "Failed: " << ex.what() << endl;;
                });
    
    std::mutex mutex;
    std::atomic_bool update_available;
};

#endif /* ShaderFileWatcher_h */
