#include "GameObject.h"

void GameObject::updateGravity()
{
	mPositionY += -0.25f;
}

void GameObject::updateCollision(std::vector<GameObject*> objects)
{
	bool isLeftCollision = false;
	bool isRightCollision = false;
	bool isDownCollision = false;

	// ¡Ø Notice: Only C++20 or later only
	auto r = objects | std::views::filter([this](GameObject* g) {return
		mPositionX - 2.0f <= g->mPositionX && g->mPositionX <= mPositionX + 2.0f &&
		mPositionY - 2.0f <= g->mPositionY && g->mPositionY <= mPositionY + 2.0f; }) | std::views::reverse;

	for (GameObject* g : r)
	{
		if (this == g)
		{
			continue;
		}

		// bottom collision
		if (g->mPositionY + 1.5f <= mPositionY)
		{
			bool isDownLeftCollision = false;
			bool isDownRightCollision = false;
			bool isPreBottomCollision = isBottomCollision;

			if (mPositionX - 1.0f <= g->mPositionX && g->mPositionX <= mPositionX + 1.0f)
			{
				isDownCollision = true;
			}

			if (!isLeftCollision && !isRightCollision)
			{
				if (g->mPositionX < mPositionX - 1.0f)
				{
					if (mPositionX - 1.8f <= g->mPositionX)
					{
						isDownLeftCollision = true;
					}
				}
				else if (mPositionX + 1.0f < g->mPositionX)
				{
					if (g->mPositionX <= mPositionX + 1.8f)
					{
						isDownRightCollision = true;
					}
				}
			}

			isBottomCollision = (isDownCollision || (isDownLeftCollision || isDownRightCollision)) ? true : false;

			if (isBottomCollision)
			{
				if (mPositionY < g->mPositionY + 2.0f)
				{
					mPositionY = g->mPositionY + 2.0f;
				}

				// jumpStatus = NO_JUMP;
				// moveStatus = STAND;
			}
			//else if ((jumpStatus == NO_JUMP) && isPreBottomCollision)
				// jumpStatus = JUMP_FALL;

		}
		// top collision
		else if (g->mPositionX - 1.0f <= mPositionX && mPositionX <= g->mPositionX + 1.0f)
		{
			if (g->mPositionY - 2.0f <= mPositionY)
			{
				mPositionY = g->mPositionY - 2.0f;
				// jumpStatus = JUMP_FALL;
			}
		}
		// right collision
		else if (mPositionX < g->mPositionX - 1.0f)
		{
			isRightCollision = true;

			if (g->mPositionX - 2.0f <= mPositionX)
			{
				mPositionX = g->mPositionX - 2.0f;
			}
		}
		// left collision
		else if (g->mPositionX + 1.0f < mPositionX)
		{
			isLeftCollision = true;

			if (mPositionX <= g->mPositionX + 2.0f)
			{
				mPositionX = g->mPositionX + 2.0f;
			}
		}
	}
}

void GameObject::draw(const int& textureNumber)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glPushMatrix();
	{
		glScalef(mTextureRate, mTextureRate, mTextureRate);

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