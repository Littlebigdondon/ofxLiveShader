//
//  ShaderFileWatcher.hpp
//  LiveShader
//
//  Created by Donnie Pitts on 3/1/19.
//

#ifndef ShaderFileWatcher_hpp
#define ShaderFileWatcher_hpp

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
    string shaderPath = "";
    string vertexShader;
    string fragmentShader;
    
    ShaderFileWatcher(string shaderPath,
                      string vertexShader = "*.vert",
                      string fragmentShader = "*.frag");
    ~ShaderFileWatcher();
    
    bool checkForUpdate();
    
private:
    string subscriptionName = "ShaderFileWatcher";
//    dynamic query = dynamic::object("fields", dynamic::array("name"))(
//                      "expression",
//                      dynamic::array("name",
//                                     dynamic::array(this->vertexShader, this->fragmentShader)));
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
    std::atomic_bool update_available = false;
};

#endif /* ShaderFileWatcher_hpp */
