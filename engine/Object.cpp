#include "Object.h"

void Object::draw(const int& textureNumber) {
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glPushMatrix();
  {
    bool isRightSide = true;
    float t;

    glScalef(mTextureRate, mTextureRate, mTextureRate);

    glBindTexture(GL_TEXTURE_2D, getTexture(isRightSide));

    glBegin(GL_QUADS);

    glColor3f(1.0f, 1.0f, 1.0f);

    t = isRightSide * 2.0f - 1.0f;

    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(mPosX - t, mPosY + 1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(mPosX - t, mPosY - 1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(mPosX + t, mPosY - 1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(mPosX + t, mPosY + 1.0f);

    glEnd();
  }
  glPopMatrix();

  glBindTexture(GL_TEXTURE_2D, 0);

  glDisable(GL_TEXTURE_2D);
}