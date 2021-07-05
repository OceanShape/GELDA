#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Game.h"

class Platformer : public Game {
 public:
  Platformer(const std::string& title, const int& width, const int& height)
      : Game(title, width, height, "./data/resource.csv",
             "./data/gameobject.dat") {}

  void getInput() override {
    if (isKeyDown(GLFW_KEY_ESCAPE))
      glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE);

    if (isKeyDown(GLFW_KEY_G)) changeMode();

    if (isKeyPressed(GLFW_KEY_LEFT))
      input(eInputStatus::LEFT);
    else if (isKeyPressed(GLFW_KEY_RIGHT))
      input(eInputStatus::RIGHT);
    else if (isKeyPressed(GLFW_KEY_UP))
      input(eInputStatus::UP);
    else if (isKeyPressed(GLFW_KEY_DOWN))
      input(eInputStatus::DOWN);
    else if (isKeyUp(GLFW_KEY_LEFT) || isKeyUp(GLFW_KEY_RIGHT))
      input(eInputStatus::ARROW_RELEASE);

    if (isEditorMode == true) {
      if (isKeyUp(GLFW_KEY_UP) || isKeyUp(GLFW_KEY_DOWN))
        input(eInputStatus::ARROW_RELEASE);
    }

    if (isKeyUp(GLFW_KEY_F))
      input(eInputStatus::JUMP_RELEASE);
    else if (isKeyDown(GLFW_KEY_F))
      input(eInputStatus::JUMP_PRESS);

    updateKeyStatus();
  }

  void update() override {
    if (isEditorMode == false) mControllable->update(isEditorMode, mObject);
  }
};

int main() {
  Platformer platformerExample("GELDA", 1024, 1024);

  platformerExample.run();

  std::cout << "See U GELDA!" << std::endl;

  return 0;
}