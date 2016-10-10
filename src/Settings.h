#ifndef GRAFIKA_SETTINGS_H
#define GRAFIKA_SETTINGS_H
#include <yaml-cpp/yaml.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>

class Settings {

 private:
  static Settings *instance;
  YAML::Node config;
  Settings();
  bool isFullscreen();
  GLFWmonitor *monitor;
  int widthMM, heightMM;

 public:
  static Settings *getInstance();
  int getWindowHeight();
  int getWindowWidth();
  glm::vec3 getStartingPoint();
  GLFWmonitor *getMonitorOrNull();

};


#endif //GRAFIKA_SETTINGS_H
