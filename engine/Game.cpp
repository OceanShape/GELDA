#include "Game.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Game::Game(const std::string& title, int width, int height, const std::string& resourceDir, const std::string& objectDir)
{
	assert(glfwInit());

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);


	glfwMakeContextCurrent(glfwWindow);
	glfwGetFramebufferSize(glfwWindow, &width, &height);
	glfwSetWindowPos(glfwWindow, (mode->width - width) / 2, (mode->height - height) / 2);


	assert(glfwWindow);
	glfwSwapInterval(1); // 60fps


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

	std::cout << "Display width = " << width << std::endl
		<< "Display height = " << height << std::endl
		<< "Aspect ratio = " << aspect_ratio << std::endl
		<< "Press Escape key to exit" << std::endl;
}

void Game::initTexture(const std::string& dir)
{
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::ifstream file(dir, std::ios_base::in);

	if (file.is_open() == false)
	{
		std::cout << "error: Reading \"" << dir << "\"failed\n" << std::endl;
		exit(-1);
	}

	std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	std::string delim("\n");

	char* next = nullptr;
	char* str = strtok_s((char*)data.c_str(), delim.c_str(), &next);
	GLuint* texture = nullptr;

	for (size_t i = 1; str != NULL; ++i)
	{
		if (str[0] == '#')
		{
			mTexture.push_back(texture);
			texture = nullptr;
			i = 0;
		}
		else
		{
			// load texture
			int width;
			int height;
			int channelCount;
			unsigned char* imageData;

			texture = (GLuint*)realloc(texture, sizeof(GLuint) * i);
			glGenTextures(1, texture + i - 1);
			imageData = stbi_load(str, &width, &height, &channelCount, 0);

			if (imageData == NULL)
			{
				std::cout << "error : Failed to load texture \"" << str << "\"" << std::endl;
				exit(-1);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, *(texture + i - 1));

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				if (channelCount == 4)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
				}
				else if (channelCount == 3)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
				}
				else
				{
					std::cout << "  notice : Use RGB or RGBA images. Your input image has " << channelCount << "channels." << std::endl;
				}

				glGenerateMipmap(GL_TEXTURE_2D);
			}


			stbi_image_free(imageData);
		}

		str = strtok_s(NULL, delim.c_str(), &next);
	}

	mTexture.push_back(texture);
}

void Game::initObject(const std::string& dir)
{
	std::ifstream file(dir, std::ios_base::in);

	if (file.is_open() == false)
	{
		std::cout << "error: Reading \"" << dir << "\"failed\n" << std::endl;
		exit(-1);
	}

	std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	std::string delim(",\n");

	if (data.size() == 0)
	{
		std::cout << "error: \"" << dir << "\" is not a valid file" << std::endl;
		exit(-1);
	}

	char* context = nullptr;
	char* str = strtok_s((char*)data.c_str(), delim.c_str(), &context);
	int buffer[4];

	for (size_t i = 0; str != NULL;)
	{
		buffer[i] = atoi(str);

		if (i == 3)
		{
			mObject.push_back(new Object(mTexture[buffer[0]] + buffer[1],
				buffer[2] * 2 + 1.0f,	// x position
				buffer[3] * 2 + 1.0f)); // y position
			i = 0;
		}
		else
		{
			++i;
		}

		str = strtok_s(NULL, delim.c_str(), &context);
	}

	// Set playable object
	mControllable = mObject[0];
}

void Game::update()
{
	// check control status
	if (isKeyDown(GLFW_KEY_ESCAPE))		glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE);

	if (isKeyDown(GLFW_KEY_G))
	{
		isEditorMode = !isEditorMode;

		mControllable->setMode(isEditorMode);
	}

	if (isEditorMode == true)
	{
		if (isKeyPressed(GLFW_KEY_LEFT))		mControllable->inputEditor(eInputStatus::LEFT);
		else if (isKeyPressed(GLFW_KEY_RIGHT))	mControllable->inputEditor(eInputStatus::RIGHT);
		else if (isKeyPressed(GLFW_KEY_UP))	mControllable->inputEditor(eInputStatus::UP);
		else if (isKeyPressed(GLFW_KEY_DOWN))	mControllable->inputEditor(eInputStatus::DOWN);
		else if (isKeyUp(GLFW_KEY_LEFT)
			|| isKeyUp(GLFW_KEY_RIGHT)
			|| isKeyUp(GLFW_KEY_UP)
			|| isKeyUp(GLFW_KEY_DOWN))		mControllable->inputEditor(eInputStatus::ARROW_RELEASE);
	}
	else
	{
		if (isKeyPressed(GLFW_KEY_LEFT))		mControllable->inputControl(eInputStatus::LEFT);
		else if (isKeyPressed(GLFW_KEY_RIGHT))	mControllable->inputControl(eInputStatus::RIGHT);
		else if (isKeyUp(GLFW_KEY_LEFT)
			|| isKeyUp(GLFW_KEY_RIGHT))		mControllable->inputControl(eInputStatus::ARROW_RELEASE);

		if (isKeyPressed(GLFW_KEY_X))		mControllable->inputControl(eInputStatus::JUMP_PRESS);
		else if (isKeyReleased(GLFW_KEY_X))	mControllable->inputControl(eInputStatus::JUMP_RELEASE);
	}

	mControllable->update(isEditorMode, mObject);

	updateKeyStatus();
}

void Game::draw()
{
	glClearColor(static_cast<float>(100) / 255.0f, static_cast<float>(149) / 255.0f, static_cast<float>(237) / 255.0f, 1);// background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);


	// draw grid line
	if (isEditorMode == true)
	{
		glLineWidth(1.0f);

		float max_length = 2.0f;
		float interval = 0.125f;
		float white[3] = { 255.0f, 255.0f ,255.0f };
		float pos[2];

		// vertical
		pos[1] = -max_length;
		for (float x = -max_length + interval; x < max_length; x += interval)
		{
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
		for (float y = -max_length + interval; y < max_length; y += interval)
		{
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
	for (Object* g : mObject)
	{
		g->draw(0);
	}

	glFlush();
}

void Game::run()
{
	while (!glfwWindowShouldClose(glfwWindow))
	{
		update();
		draw();

		updateKeyStatus();

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}

	glfwTerminate();
}

void Game::updateKeyStatus()
{
	for (const auto& [key, value] : keyStatus)
	{
		keyStatus[key] = glfwGetKey(glfwWindow, key) == GLFW_PRESS;
	}
}

bool Game::isKeyPressed(const int& key)
{
	return glfwGetKey(glfwWindow, key) == GLFW_PRESS;
}

bool Game::isKeyReleased(const int& key)
{
	return glfwGetKey(glfwWindow, key) == GLFW_RELEASE;
}

bool Game::isKeyUp(const int& key)
{
	return glfwGetKey(glfwWindow, key) == GLFW_RELEASE && keyStatus[key] == true;
}

bool Game::isKeyDown(const int& key)
{
	return glfwGetKey(glfwWindow, key) == GLFW_PRESS && keyStatus[key] == false;
}