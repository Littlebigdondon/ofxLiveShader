//
//  ShaderFileWatcher.cpp
//  LiveShader
//
//  Created by Donnie Pitts on 3/1/19.
//

#include "ShaderFileWatcher.h"

using namespace folly;
using namespace watchman;

ShaderFileWatcher::ShaderFileWatcher(string shaderPath,
                                     string vertexShader,
                                     string fragmentShader,
                                     string executablesPath):
    shaderPath(shaderPath),
    vertexShader(vertexShader),
    fragmentShader(fragmentShader),
    executablesPath(executablesPath) {
        
        this->eventBaseThread = std::make_shared<folly::EventBaseThread>();
        folly::EventBase* eventBase = this->eventBaseThread->getEventBase();
        
        // Add the Watchman executable path to Xcode's $PATH env variable
        // (Sadly, $PATH from here does not match our system's $PATH)
        string currPATH = getenv("PATH");
        setenv("PATH", (currPATH + ":" + executablesPath).c_str(), 1);
        
        // Create Watchman client
        this->client = std::make_shared<WatchmanClient>(eventBase,
                                                        Optional<std::string>(),
                                                        nullptr,
                                                        this->errorCallback);
        // Connect to watchman server
        auto version = this->client->connect()
        .wait()
        .value()["version"];
        
        std::cout << "Connected to Watchman: " << version << std::endl;
        
        // Watch shader directory
        this->client->watch(shaderPath)
        .then([&, this](WatchPathPtr watch) {
            std::cout << "Watching: " << shaderPath << std::endl;
            // Subscribe to notifications on changes to files within the directory
            return this->client->subscribe(this->query,
                                           watch,
                                           eventBase,
                                           [this](Try<dynamic>&& data){
                                               if (data.hasValue()) {
                                                   // Raise flag
                                                   std::unique_lock<std::mutex> lock(this->mutex);
                                                   this->update_available = true;
                                               } else {
                                                   std::cout << "subscribe() failed with: " << data.exception() << std::endl;
                                               }
                                           }).value();
        })
        .wait();
}

ShaderFileWatcher::~ShaderFileWatcher() {
    this->client->close();
    this->eventBaseThread->stop();
}

bool ShaderFileWatcher::checkForUpdate() {
    auto update = false;
    if (this->update_available) {
        update = true;
        std::unique_lock<std::mutex> lock(this->mutex);
        this->update_available = false;
    }
    return update;
}
