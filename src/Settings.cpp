//
// Created by atka on 30.08.16.
//

#include <yaml-cpp/yaml.h>
#include <iostream>
#include "Settings.h"


Settings *Settings::instance(nullptr);

Settings::Settings() {
    this -> config = YAML::LoadFile("settings.yaml");
    if(config!= nullptr)
        std::cout<<"settings loaded"<<std::endl;
}

Settings *Settings::getInstance() {
    if (Settings::instance == nullptr) {
        instance = new Settings();
    }
    return instance;
}

int Settings::getWindowHeight() {
    return config["settings"]["window"]["height"].as<int>();
}

int Settings::getWindowWidth() {
    return config["settings"]["window"]["width"].as<int>();
}



