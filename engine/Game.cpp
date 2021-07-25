#include "Game.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Game::Game(const std::string& title, int width, int height,
           const std::string& resourceDir, const std::string& objectDir) {
  assert(glfwInit());

  const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

  glfwWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

  glfwMakeContextCurrent(glfwWindow);
  glfwGetFramebufferSize(glfwWindow, &width, &height);
  glfwSetWindowPos(glfwWindow, (mode->width - width) / 2,
                   (mode->height - height) / 2);

  assert(glfwWindow);
  glfwSwapInterval(1);  // 60fps

  // Init GLEW
  glewExperimental = true;
  assert(glewInit() == GLEW_OK);

  glViewport(0, 0, width, height);
  const float aspect_ratio = (float)width / (float)height;
  glOrtho(-aspect_ratio, aspect_ratio, -1.0, 1.0, -1.0, 1.0);

  // Init textures
  initTexture(resourceDir);

  // Init GameObejcts
  initObject(objectDir);

  // Init MessageQueue
  MessageQueue::instance();

  std::cout << "Display width = " << width << std::endl
            << "Display height = " << height << std::endl
            << "Aspect ratio = " << aspect_ratio << std::endl
            << "Press Escape key to exit" << std::endl;
}

void Game::initTexture(const std::string& dir) {
  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  std::ifstream file(dir, std::ios_base::in);

  if (file.is_open() == false) {
    std::cout << "error: Reading \"" << dir << "\"failed\n" << std::endl;
    exit(-1);
  }

  std::string data((std::istreambuf_iterator<char>(file)),
                   std::istreambuf_iterator<char>());
  std::string delim("\n");

  char* next = nullptr;
  char* str = strtok_s((char*)data.c_str(), delim.c_str(), &next);

  std::vector<GLuint>* texture = new std::vector<GLuint>;

  while (str != NULL) {
    if (str[0] == '.') {
      // load texture
      int width;
      int height;
      int channelCount;
      unsigned char* imageData;

      GLuint text;
      glGenTextures(1, &text);
      imageData = stbi_load(str, &width, &height, &channelCount, 0);

      if (imageData == NULL) {
        std::cout << "error : Failed to load texture \"" << str << "\""
                  << std::endl;
        exit(-1);
      } else {
        glBindTexture(GL_TEXTURE_2D, text);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        if (channelCount == 4) {
          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                       GL_UNSIGNED_BYTE, imageData);
        } else if (channelCount == 3) {
          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                       GL_UNSIGNED_BYTE, imageData);
        } else {
          std::cout
              << "  notice : Use RGB or RGBA images. Your input image has "
              << channelCount << "channels." << std::endl;
        }

        glGenerateMipmap(GL_TEXTURE_2D);
      }

      stbi_image_free(imageData);

      texture->push_back(text);
    } else {
      mTexture.push_back(texture);
      texture = new std::vector<GLuint>;
    }

    str = strtok_s(NULL, delim.c_str(), &next);
  }

  mTexture.push_back(texture);
}

void Game::initObject(const std::string& dir) {
  std::ifstream file(dir, std::ios_base::in || std::ios_base::binary);
  std::string data;

  if (file.is_open() == false) {
    std::cout << "error: Reading \"" << dir << "\"failed\n" << std::endl;
    exit(-1);
  }

  data.resize(21);
  file.read(&data[0], data.size());

  if (file.fail() == true || data != "GELDA GAMEOBJECT FILE") {
    std::cout << "error: gameobject.dat is invalid" << std::endl;
    exit(-1);
  }

  file.seekg(32, std::ios::beg);
  data.resize(256);
  file.read(&data[0], data.size());

  const char* cp = data.c_str();

  for (int i = 0; i < data.size(); ++i) {
    char c = *cp++;
    if (c == 1) {
      Object* o = new MoveableObject(mTexture[0],
                                     (i % 16 - 8) * 2 + 1.0f,   // x position
                                     (i / 16 - 8) * 2 + 1.0f);  // y position
      mObject.push_back(o);
      mControllable = o;
    } else if (c != 0) {
      mObject.push_back(new Object(mTexture[c - 1],
                                   (i % 16 - 8) * 2 + 1.0f,    // x position
                                   (i / 16 - 8) * 2 + 1.0f));  // y position
    }
  }

  if (mControllable == nullptr) {
    std::cout << "error: Controllable object is not founded" << std::endl;
    exit(-1);
  }
}

void Game::draw() {
  glClearColor(static_cast<float>(100) / 255.0f,
               static_cast<float>(149) / 255.0f,
               static_cast<float>(237) / 255.0f, 1);  // background color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);

  // draw grid line
  if (isEditorMode == true) {
    glLineWidth(1.0f);

    float max_length = 2.0f;
    float interval = 0.125f;
    float white[3] = {255.0f, 255.0f, 255.0f};
    float pos[2];

    // vertical
    pos[1] = -max_length;
    for (float x = -max_length + interval; x < max_length; x += interval) {
      pos[0] = x;
      glBegin(GL_LINES);
      {
        glColor3fv(white);
        glVertex2fv(pos);
        pos[1] = -pos[1];
        glVertex2fv(pos);
      }
      glEnd();
    }

    // horizontal
    pos[0] = -max_length;
    for (float y = -max_length + interval; y < max_length; y += interval) {
      pos[1] = y;
      glBegin(GL_LINES);
      {
        glColor3fv(white);
        glVertex2fv(pos);
        pos[0] = -pos[0];
        glVertex2fv(pos);
      }
      glEnd();
    }
  }

  // draw game objects
  for (Object* g : mObject) {
    g->draw(0);
  }

  glFlush();
}

void Game::run() {
  while (!glfwWindowShouldClose(glfwWindow) && isGameOver == false) {
    getInput();
    update();
    draw();

    glfwSwapBuffers(glfwWindow);
    glfwPollEvents();
  }

  glfwTerminate();
}

bool Game::processMessage() {
  while (MessageQueue::getSize() > 0) {
    const CollisionMessage msg = MessageQueue::front();
    msg.object->updatePosition(msg.target, msg.type);
    MessageQueue::pop();
  }
  return false;
}

void Game::updateKeyStatus() {
  for (const auto& [key, value] : keyStatus) {
    keyStatus[key] = glfwGetKey(glfwWindow, key) == GLFW_PRESS;
  }
}

bool Game::isKeyPressed(const int& key) {
  return glfwGetKey(glfwWindow, key) == GLFW_PRESS;
}

bool Game::isKeyReleased(const int& key) {
  return glfwGetKey(glfwWindow, key) == GLFW_RELEASE;
}

bool Game::isKeyUp(const int& key) {
  return glfwGetKey(glfwWindow, key) == GLFW_RELEASE && keyStatus[key] == true;
}

bool Game::isKeyDown(const int& key) {
  return glfwGetKey(glfwWindow, key) == GLFW_PRESS && keyStatus[key] == false;
}