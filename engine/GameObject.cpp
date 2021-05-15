#include "GameObject.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float gTextureSize = 0.0625f;

extern float positionX;
extern float positionY;

void GameObject::initTexture(const std::string fileName[])
{
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int width;
	int height;
	int numberChannel;
	unsigned char* data = nullptr;
	mTexture = new GLuint[mTextureCount];
	glGenTextures(mTextureCount, mTexture);

	for (int count = 0; count < mTextureCount; ++count)
	{
		data = stbi_load(fileName[count].c_str(), &width, &height, &numberChannel, 0);

		if (data)
		{
			glBindTexture(GL_TEXTURE_2D, mTexture[count]);

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
			std::cout << "  notice : Failed to load texture \"" << fileName[count] << "\"" << std::endl;
		}

		stbi_image_free(data);
	}
}

void GameObject::draw(int textureNumber)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	{
		glScalef(gTextureSize, gTextureSize, gTextureSize);

		glBindTexture(GL_TEXTURE_2D, mTexture[textureNumber]);

		glBegin(GL_QUADS);

		glColor3f(1.0f, 1.0f, 1.0f);

		glTexCoord2f(0.0f, 0.0f); glVertex2f(positionX - 1.0f, positionY + 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(positionX - 1.0f, positionY - 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(positionX + 1.0f, positionY - 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(positionX + 1.0f, positionY + 1.0f);

		glEnd();
	}
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
}