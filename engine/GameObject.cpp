#include "GameObject.h"

void GameObject::draw(const int& textureNumber)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	{
		glScalef(mTextureSize, mTextureSize, mTextureSize);

		glBindTexture(GL_TEXTURE_2D, mTexture[textureNumber]);

		glBegin(GL_QUADS);

		glColor3f(1.0f, 1.0f, 1.0f);

		glTexCoord2f(0.0f, 0.0f); glVertex2f(mPositionX - 1.0f, mPositionY + 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(mPositionX - 1.0f, mPositionY - 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(mPositionX + 1.0f, mPositionY - 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(mPositionX + 1.0f, mPositionY + 1.0f);

		glEnd();
	}
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
}