#include "Resource.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float gTextureSize = 0.0625f;

void Resource::init(const std::string fileName[])
{
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	mTexture = new GLuint[mTextureCount];
	glGenTextures(mTextureCount, mTexture);

	unsigned char* data = nullptr;
	int width;
	int height;
	int number_channel;

	for (int count = 0; count < mTextureCount; count++)
	{
		data = stbi_load(fileName[count].c_str(), &width, &height, &number_channel, 0);

		if (data == NULL)
		{
			std::cout << "  notice : Failed to load texture \"" << fileName[count] << "\"" << std::endl;
		}

		glBindTexture(GL_TEXTURE_2D, mTexture[count]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		if (number_channel == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else if (number_channel == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "  notice : Use RGB or RGBA images. Your input image has " << number_channel << "channels." << std::endl;
		}

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

}

void Resource::draw(int textureNumber)
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

		glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);

		glEnd();
	}
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
}