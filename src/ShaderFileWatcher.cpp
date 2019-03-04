//
//  ShaderFileWatcher.cpp
//  LiveShader
//
//  Created by Donnie Pitts on 3/1/19.
//

#include "ShaderFileWatcher.hpp"

using namespace folly;
using namespace watchman;

ShaderFileWatcher::ShaderFileWatcher(string shaderPath,
                                     string vertexShader,
                                     string fragmentShader):
    shaderPath(shaderPath),
    vertexShader(vertexShader),
    fragmentShader(fragmentShader) {
        
        this->eventBaseThread = std::make_shared<folly::EventBaseThread>();
        folly::EventBase* eventBase = this->eventBaseThread->getEventBase();
        
        // Add the Watchman executable path to Xcode's $PATH env variable
        // (Sadly, $PATH from here does not match our system's $PATH)
        string currPATH = getenv("PATH");
        string executablesPath = "/usr/local/bin/";
        setenv("PATH", (currPATH + ":" + executablesPath).c_str(), 1);
        
        // Create Watchman client
        this->client = std::make_shared<WatchmanClient>(eventBase,
                                                        Optional<std::string>(),
                                                        nullptr,
                                                        this->errorCallback);
        // Connect to watchman server
        this->client->connect().then([&, this](dynamic&& response) {
            std::cout << "Connected to Watchman: " << response["version"] << std::endl;
            // Watch shader directory
            return this->client->watch(shaderPath).then([&, this](WatchPathPtr watch) {
                // Subscribe to notifications on changes to files within the directory
                return this->client->subscribe(this->query,
                                               watch,
                                               eventBase,
                                               [this](Try<dynamic>&& data){
                                                   if (data.hasValue()) {
                                                       // Raise flag
                                                       std::unique_lock<std::mutex> lock(mutex);
                                                       this->update_available = true;
                                                   } else {
                                                       std::cout << "subscribe() failed with: " << data.exception() << std::endl;
                                                   }
                                               });
            });
        }).wait();
}

ShaderFileWatcher::~ShaderFileWatcher() {
    this->client->close();
    this->eventBaseThread->stop();
}

bool ShaderFileWatcher::checkForUpdate() {
    auto update = false;
    if (this->update_available) {
        update = true;
        std::unique_lock<std::mutex> lock(mutex);
        this->update_available = false;
    }
    return update;
}
