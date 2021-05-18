#include <iostream>

#include "TextureManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureManager* TextureManager::mInstance = nullptr;
std::vector<GLuint*> TextureManager::mTextures;

GLuint* TextureManager::getTexture(const int& objectNumber)
{
	if (mInstance == nullptr) mInstance = new TextureManager();

	return mInstance->mTextures[objectNumber];
}

TextureManager::TextureManager()
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
		mTextures.push_back(new GLuint[textureCount[i]]);
		glGenTextures(textureCount[i], mTextures[i]);

		for (int j = 0; j < textureCount[i]; ++j)
		{
			unsigned char* data = stbi_load(name[i][j].c_str(), &width, &height, &numberChannel, 0);

			if (data)
			{
				glBindTexture(GL_TEXTURE_2D, mTextures[i][j]);

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