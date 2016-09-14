#include <yaml-cpp/yaml.h>
#include <iostream>
#include "Settings.h"

Settings *Settings::instance(nullptr);

Settings::Settings() {
  this->config = YAML::LoadFile("settings.yaml");

  this->monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *mode = glfwGetVideoMode(this->monitor);
  this->widthMM = mode->width;
  this->heightMM = mode->height;



  if (config != nullptr)
    std::cout << "settings loaded" << std::endl;
}

Settings *Settings::getInstance() {
  if (Settings::instance == nullptr) {
    instance = new Settings();
  }
  return instance;
}

int Settings::getWindowHeight() {
  return isFullscreen() ? this->heightMM : config["settings"]["window"]["height"].as<int>();
}

int Settings::getWindowWidth() {
  return isFullscreen() ? this->widthMM : config["settings"]["window"]["width"].as<int>();
}

bool Settings::isFullscreen() {
  return config["settings"]["window"]["fullscreen"].as<bool>();
}

GLFWmonitor *Settings::getMonitorOrNull() {
  return isFullscreen() ? monitor : NULL;
}
float Settings::getMouseSpeed() {
  return config["settings"]["player"]["mouse speed"].as<float>();
}
float Settings::getMoveSpeed() {
  return config["settings"]["player"]["move speed"].as<float>();
}
float Settings::getStartingPointX() {
  return config["settings"]["player"]["starting point"]["x"].as<float>();
}
float Settings::getStartingPointY() {
  return config["settings"]["player"]["starting point"]["y"].as<float>();
}
float Settings::getStartingPointZ() {
  return config["settings"]["player"]["starting point"]["z"].as<float>();
}























