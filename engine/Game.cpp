#include "Game.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Game::Game(const std::string title, int width, int height)
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
	initTexture();

	//1 3 5 7 9 11 13 15 | 17 19 21 23 25 27 29 31
	//0 1 2 3 4  5  6  7 |  8  9 10 11 12 13 14 15

	// Init GameObejcts
	mGameObject.push_back(new GameObject(mTexture[0], 0.0f, 0.0f));

	mGameObject.push_back(new GameObject(mTexture[1] + 2, 4.0f, 0.0f));

	for (int i = 0; i < 16; ++i)
	{
		mGameObject.push_back(new GameObject(mTexture[1] + 0, -16.0f + i * 2 + 1.0f, -16.0f + 1.0f));
	}
	for (int i = 0; i < 16; ++i)
	{
		mGameObject.push_back(new GameObject(mTexture[1] + 1, -16.0f + i * 2 + 1.0f, -16.0f + 3.0f));
	}

	// Set playable
	mControllable = mGameObject[0];

	printf("Display width = %d\n", width);
	printf("Display height = %d\n", height);
	printf("Aspect ratio = %f\n", aspect_ratio);
	printf("Press Escape key to exit");
}

void Game::initTexture()
{
	char* fileData;
	int fileSize;
	FILE* fp;

	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (fopen_s(&fp, "./Object.info", "r") != 0)
	{
		exit(-1);
	}

	{
		fseek(fp, 0L, SEEK_END);
		fileSize = ftell(fp);
		fileData = new char[fileSize];
		memset(fileData, 0, fileSize);
		fseek(fp, 0L, SEEK_SET);
		fread_s(fileData, sizeof(char) * fileSize, 1, sizeof(char) * fileSize, fp);
		fclose(fp);
	}

	{
		char buffer[LENGTH];
		char* pData = fileData;
		char* pBuffer = buffer;
		int count = 1;
		GLuint* texture = nullptr;

		memset(buffer, NULL, LENGTH);

		while (true)
		{
			while (*pData != '\n' && *pData != '\0')
			{
				*pBuffer = *pData;
				++pBuffer;
				++pData;
			}
			*pBuffer = '\0';
			pBuffer = buffer;

			// load texture
			{
				int width;
				int height;
				int numberChannel;
				unsigned char* imageData;

				texture = (GLuint*)realloc(texture, sizeof(GLuint) * count);
				glGenTextures(1, texture + count - 1);
				imageData = stbi_load(pBuffer, &width, &height, &numberChannel, 0);

				if (imageData)
				{
					glBindTexture(GL_TEXTURE_2D, *(texture + count - 1));

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

					if (numberChannel == 4)
					{
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
					}
					else if (numberChannel == 3)
					{
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
					}
					else
					{
						std::cout << "notice : Use RGB or RGBA images. Your input image has " << numberChannel << "channels." << std::endl;
					}

					glGenerateMipmap(GL_TEXTURE_2D);
				}
				else
				{
					std::cout << "error : Failed to load texture \"" << pBuffer << "\"" << std::endl;
					exit(-1);
				}

				stbi_image_free(imageData);
			}

			if (*pData == '\0')
			{
				mTexture.push_back(texture);
				break;
			}

			++pData;

			if (*pData == '\n')
			{
				++pData;
				mTexture.push_back(texture);
				texture = nullptr;
				count = 1;
			}
			else
			{
				++count;
			}
		}
	}

	delete[] fileData;
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


	// update game object position
	mControllable->move(moveX, moveY);

	// update game object physics status
	mControllable->updateGravity();
	mControllable->updateCollision(mGameObject);


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
	{
		g->draw(0); // static texture number 0 for test
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