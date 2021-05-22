#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <vector>

#include "CollisionDirection.h"

class GameObject
{
private:
	GLuint* mTexture;
	int mTextureCount;
	float mTextureRate = 0.0625f;


	float mPositionX = 0.0f;
	float mPositionY = 0.0f;


public:
	GameObject(GLuint* texture, const float& posX, const float& posY)
	{
		mTextureCount = 1; // for Test
		mTexture = texture;
		mPositionX = posX;
		mPositionY = posY;
	}

	~GameObject()
	{
	}

	void draw(const int& textureNumber);
	void move(const float& moveX, const float& moveY)
	{
		mPositionX += moveX;
		mPositionY += moveY;
	}

	int isCollision(GameObject* obj, std::vector<GameObject*> other)
	{
		//(x1, y1)------(x2, y1)
		//	|			    |
		//	|			    |
		//	|			    |
		//	|				|
		//(x1, y2)------(x2, y2)
		
		float r1x1 = mPositionX - 1.0f;
		float r1x2 = mPositionX + 1.0f;
		float r1y1 = mPositionY + 1.0f;
		float r1y2 = mPositionY - 1.0f;

		float r2x1 = obj->mPositionX - 1.0f;
		float r2x2 = obj->mPositionX + 1.0f;
		float r2y1 = obj->mPositionY + 1.0f;
		float r2y2 = obj->mPositionY - 1.0f;


		/*
		bool is_left_block = false;
		bool is_right_block = false;
		bool is_bottom_collision = false;

		
		for (GameObject* g : other)
		{
			if (obj->mPositionX == g->mPositionX && obj->mPositionY == g->mPositionY)
			{
				continue;
			}

			if (obj->mPositionX - 1.0f > g->mPositionX && obj->mPositionX - 3.0f < g->mPositionX &&
				obj->mPositionY - 1.0f > g->mPositionY && obj->mPositionY - 3.0f < g->mPositionY)
			{
				
			}

			if (obj->mPositionX - 1.0f > g->mPositionX && obj->mPositionX + 1.0f < g->mPositionX &&
				obj->mPositionY - 1.0f > g->mPositionY && obj->mPositionY - 3.0f < g->mPositionY)
			{

			}

			if (obj->mPositionX - 1.0f > g->mPositionX && obj->mPositionX + 1.0f < g->mPositionX &&
				obj->mPositionY - 1.0f > g->mPositionY && obj->mPositionY - 3.0f < g->mPositionY)
			{

			}
		}


		for (int i = 2; i >= 0; i--)
		{
			for (int j = 0; j < 3; j++)
			{
				if (i == 1 && j == 1)
				{
					continue;
				}
				
				if (x_index - 1 + j < 0)
				{
					is_left_block = true;
					if (center.x < x_index * unit_size - 1.0f + htbox.side.x / 2)
					{
						center.x = x_index * unit_size - 1.0f + htbox.side.x / 2;
						htbox.update(center, size);
					}
				}
				else if (map.x_total_size <= x_index - 1 + j)
				{
					is_right_block = true;
					if (center.x > (x_index + 1) * unit_size - 1.0f - htbox.side.x / 2)
					{
						center.x = (x_index + 1) * unit_size - 1.0f - htbox.side.x / 2;
						htbox.update(center, size);
					}
				}


				if (y_index - 1 + i < 0 || map.y_size <= y_index - 1 + i) { continue; }

				if (i == 0 && j == 1)
				{

					bool is_bottom_right_collision = false;
					bool is_bottom_left_collision = false;
					bool pre_is_bottom_total_collision = is_bottom_total_collision;

					is_bottom_collision = map.htboxList[y_index - 1 + i][x_index - 1 + j] ? true : false;

					if (is_left_block || is_right_block)
					{
					}
					else if (map.htboxList[y_index - 1 + i][x_index - 1 + j - 1])
					{
						if (x_dec <= 0.25f)
						{
							is_bottom_left_collision = true;
						}
					}
					else if (map.htboxList[y_index - 1 + i][x_index - 1 + j + 1])
					{
						if (x_dec >= 0.75f)
						{
							is_bottom_right_collision = true;
						}
					}

					if (is_bottom_collision || (is_bottom_left_collision || is_bottom_right_collision))
					{
						is_bottom_total_collision = true;
					}
					else
					{
						is_bottom_total_collision = false;
					}


					if (is_bottom_total_collision)
					{
						if (center.y < y_index * unit_size - 1.0f + htbox.side.y / 2 - 0.01f)
						{
							center.y = y_index * unit_size - 1.0f + htbox.side.y / 2 - 0.01f;

							time_check = 0.0f;
							y_start_center = center.y;
							if (jump_sound)
							{
								characterSound.stopSound("jump");
								jump_sound = false;
							}

							jumpStatus = eJumpStatus::NO_JUMP;
							moveStatus = eMoveStatus::STAND;

						}
					}
					else if ((jumpStatus == eJumpStatus::NO_JUMP) && pre_is_bottom_total_collision)
					{
						jumpStatus = eJumpStatus::JUMP_FALL;
					}

					htbox.update(center, size);
				}
				else if (i == 2 && j == 1)
				{
					if (map.htboxList[y_index - 1 + i][x_index - 1 + j])
					{
						if (center.y > (y_index + 1) * unit_size - 1.0f - htbox.side.y / 2)
						{
							center.y = (y_index + 1) * unit_size - 1.0f - htbox.side.y / 2;

							characterSound.stopSound("jump");

							jumpStatus = eJumpStatus::JUMP_FALL;

							htbox.update(center, size);
						}
					}
					continue;
				}

				else if (i == 1 && j == 2)
				{
					if (map.htboxList[y_index - 1 + i][x_index - 1 + j])
					{
						is_right_block = true;
						if (center.x > (x_index + 1) * unit_size - 1.0f - htbox.side.x / 2)
						{
							center.x = (x_index + 1) * unit_size - 1.0f - htbox.side.x / 2;
							htbox.update(center, size);
						}
					}
					else
					{
						is_right_block = false;
					}
				}
				else if (i == 1 && j == 0)
				{
					if (map.htboxList[y_index - 1 + i][x_index - 1 + j])
					{
						is_left_block = true;
						if (center.x < x_index * unit_size - 1.0f + htbox.side.x / 2)
						{
							center.x = x_index * unit_size - 1.0f + htbox.side.x / 2;
							htbox.update(center, size);
						}
					}
					else
					{
						is_left_block = false;
					}
				}
			}
		}
		*/

		if (
			((r1x1 <= r2x1 && r2x1 <= r1x2) || (r1x1 <= r2x2 && r2x2 <= r1x2))
			&& ((r1y2 <= r2y1 && r2y1 <= r1y1) || (r1y2 <= r2y2 && r2y2 <= r1y1))
			)
		{
			std::cout << "collision detected" << std::endl;
			return true;
		}
		else
		{
			return false;
		}
	}

};