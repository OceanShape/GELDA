#pragma once
enum class eDirectionStatus
{
	LEFT,
	RIGHT
};

enum class eMoveStatus
{
	STOP,
	MOVE
};

enum class eJumpStatus
{
	NO_JUMP,
	JUMP,
	JUMP_DECELERATION,
	FALL
};