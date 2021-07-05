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

    if (isKeyDown(GLFW_KEY_G)) {
      isEditorMode = !isEditorMode;

      mControllable->setMode(isEditorMode);
    }

    if (isEditorMode == true) {
      if (isKeyPressed(GLFW_KEY_LEFT))
        mControllable->inputEditor(eInputStatus::LEFT);
      else if (isKeyPressed(GLFW_KEY_RIGHT))
        mControllable->inputEditor(eInputStatus::RIGHT);
      else if (isKeyPressed(GLFW_KEY_UP))
        mControllable->inputEditor(eInputStatus::UP);
      else if (isKeyPressed(GLFW_KEY_DOWN))
        mControllable->inputEditor(eInputStatus::DOWN);
      else if (isKeyUp(GLFW_KEY_LEFT) || isKeyUp(GLFW_KEY_RIGHT) ||
               isKeyUp(GLFW_KEY_UP) || isKeyUp(GLFW_KEY_DOWN))
        mControllable->inputEditor(eInputStatus::ARROW_RELEASE);
    } else {
      if (isKeyPressed(GLFW_KEY_LEFT))
        mControllable->inputControl(eInputStatus::LEFT);
      else if (isKeyPressed(GLFW_KEY_RIGHT))
        mControllable->inputControl(eInputStatus::RIGHT);
      else if (isKeyUp(GLFW_KEY_LEFT) || isKeyUp(GLFW_KEY_RIGHT))
        mControllable->inputControl(eInputStatus::ARROW_RELEASE);

      if (isKeyUp(GLFW_KEY_F)) {
        mControllable->inputControl(eInputStatus::JUMP_RELEASE);
      } else if (isKeyDown(GLFW_KEY_F)) {
        mControllable->inputControl(eInputStatus::JUMP_PRESS);
      }
    }

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