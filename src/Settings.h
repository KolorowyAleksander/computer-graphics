//
// Created by atka on 30.08.16.
//

#ifndef GRAFIKA_SETTINGS_H
#define GRAFIKA_SETTINGS_H
#include <yaml-cpp/yaml.h>

class Settings {

 private:
    static Settings *instance;
    YAML::Node config;
    Settings();

 public:
    static Settings *getInstance();
    int getWindowHeight();
    int getWindowWidth();
};


#endif //GRAFIKA_SETTINGS_H
