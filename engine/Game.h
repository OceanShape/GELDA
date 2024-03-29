#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "MessageQueue.h"
#include "MoveableObject.h"
#include "Object.h"

class Game {
 protected:
  GLFWwindow* glfwWindow = nullptr;
  std::vector<Object*> mObject;

  std::vector<std::vector<GLuint>*> mTexture;
  std::map<int, bool> keyStatus;

  bool isGameOver = false;
  bool isEditorMode = true;
  MoveableObject* mControllable = nullptr;

 public:
  Game(const std::string& title, int width, int height,
       const std::string& resourceDir, const std::string& objectDir);

  ~Game() {
    for (Object* o : mObject) {
      delete o;
    }

    for (std::vector<GLuint>* t : mTexture) {
      delete t;
    }
  }

  void run();  // Entire game loop
  void reportErrorAndExit(const std::string& function,
                          const std::string& message);

 private:
  void draw();                               // Draw all game objects
  void initTexture(const std::string& dir);  // Initialize textures
  void initObject(
      const std::string& dir);  // Initialize game objects(texture, position)
  void update() {               // Update game object status
    if (isEditorMode == false)
      for (Object* o : mObject) o->update(mObject);

    if (mControllable->mLifeStatus == eLifeStatus::DEAD) {
      isGameOver = true;
      return;
    }
    isGameOver = processMessage();
  };
  bool processMessage();
  
 protected:
  void input(const eInputStatus& inputStatus) {
    mControllable->input(isEditorMode, inputStatus);
  }
  virtual void getInput(){};
  void changeMode() {
    isEditorMode = !isEditorMode;
    for (Object* o : mObject) o->setMode(isEditorMode);
  };
  void updateKeyStatus();
  bool isKeyPressed(const int& key);
  bool isKeyReleased(const int& key);
  bool isKeyUp(const int& key);
  bool isKeyDown(const int& key);
};