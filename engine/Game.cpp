#include "Game.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Game::Game(const std::string title, int width, int height)
{
	assert(glfwInit());

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);


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
	initTexture();

	//1 3 5 7 9 11 13 15 | 17 19 21 23 25 27 29 31
	//0 1 2 3 4  5  6  7 |  8  9 10 11 12 13 14 15

	// Init GameObejcts
	mGameObject.push_back(new GameObject(mTexture[0], 0.0f, 0.0f));

	mGameObject.push_back(new GameObject(mTexture[1], 4.0f, 0.0f));

	for (int i = 0; i < 16; ++i)
		mGameObject.push_back(new GameObject(mTexture[1], -16.0f + i * 2 + 1.0f, -16.0f + 1.0f));
	for (int i = 0; i < 16; ++i)
		mGameObject.push_back(new GameObject(mTexture[1], -16.0f + i * 2 + 1.0f, -16.0f + 3.0f));

	// Set playable
	mControllable = mGameObject[0];


	std::cout << "Display width = " << width << std::endl
		<< "Display height = " << height << std::endl
		<< "Aspect ratio = " << aspect_ratio << std::endl;
	std::cout << "Press Escape key to exit" << std::endl;
}

void Game::initTexture()
{
	int objectCount = 2;
	int textureCount[] = { 1, 1 };
	std::string name[][1] =
	{
		{ "./resource/sample_playable.png" },
		{ "./resource/sample_ground.png" }
	};


	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int width;
	int height;
	int numberChannel;


	for (int i = 0; i < objectCount; ++i)
	{
		mTexture.push_back(new GLuint[textureCount[i]]);
		glGenTextures(textureCount[i], mTexture[i]);

		for (int j = 0; j < textureCount[i]; ++j)
		{
			unsigned char* data = stbi_load(name[i][j].c_str(), &width, &height, &numberChannel, 0);

			if (data)
			{
				glBindTexture(GL_TEXTURE_2D, mTexture[i][j]);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				if (numberChannel == 4)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				}
				else if (numberChannel == 3)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				}
				else
				{
					std::cout << "  notice : Use RGB or RGBA images. Your input image has " << numberChannel << "channels." << std::endl;
				}

				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				std::cout << "  notice : Failed to load texture \"" << name[i] << "\"" << std::endl;
			}

			stbi_image_free(data);
		}
	}
}

void Game::update()
{
	// check control status
	if (isKeyDown(GLFW_KEY_ESCAPE))		glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE);

	if (isKeyDown(GLFW_KEY_LEFT))		moveX = -delta;
	else if (isKeyUp(GLFW_KEY_LEFT))	moveX = 0.0f;
	else if (isKeyDown(GLFW_KEY_RIGHT))	moveX = delta;
	else if (isKeyUp(GLFW_KEY_RIGHT))	moveX = 0.0f;
	else if (isKeyDown(GLFW_KEY_UP))	moveY = delta;
	else if (isKeyUp(GLFW_KEY_UP))		moveY = 0.0f;
	else if (isKeyDown(GLFW_KEY_DOWN))	moveY = -delta;
	else if (isKeyUp(GLFW_KEY_DOWN))	moveY = 0.0f;

	if (isKeyDown(GLFW_KEY_G))			isDrawGrid = !isDrawGrid;


	// update game object collision
	mControllable->isCollision(mGameObject);

	// update game object position
	mControllable->move(moveX, moveY);

	updateKeyStatus();
}

void Game::draw()
{
	glClearColor(static_cast<float>(100) / 255.0f, static_cast<float>(149) / 255.0f, static_cast<float>(237) / 255.0f, 1);// background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);


	// draw grid line
	if (isDrawGrid)
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
	for (GameObject* g : mGameObject)
		g->draw(0); // static texture number 0 for test

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
		if (glfwGetKey(glfwWindow, key) == GLFW_PRESS)
		{
			keyStatus[key] = true;
		}
		else if (glfwGetKey(glfwWindow, key) == GLFW_RELEASE)
		{
			keyStatus[key] = false;
		}
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
	return glfwGetKey(glfwWindow, key) == GLFW_RELEASE && keyStatus[key];
}

bool Game::isKeyDown(const int& key)
{
	return glfwGetKey(glfwWindow, key) == GLFW_PRESS && !keyStatus[key];
}