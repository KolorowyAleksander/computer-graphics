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







