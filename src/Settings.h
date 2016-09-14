#ifndef GRAFIKA_SETTINGS_H
#define GRAFIKA_SETTINGS_H
#include <yaml-cpp/yaml.h>
#include <GLFW/glfw3.h>

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
  float getStartingPointX();
  float getStartingPointY();
  float getStartingPointZ();
  float getMoveSpeed();
  float getMouseSpeed();

  GLFWmonitor *getMonitorOrNull();

};


#endif //GRAFIKA_SETTINGS_H
