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

  bool isEditorMode = true;
  Object* mControllable = nullptr;

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
 private:
  void draw();                               // Draw all game objects
  void initTexture(const std::string& dir);  // Initialize textures
  void initObject(
      const std::string& dir);  // Initialize game objects(texture, position)

 protected:
  virtual void getInput(){};
  virtual void update(){};  // Update input and game object status
  void updateKeyStatus();
  bool isKeyPressed(const int& key);
  bool isKeyReleased(const int& key);
  bool isKeyUp(const int& key);
  bool isKeyDown(const int& key);
};