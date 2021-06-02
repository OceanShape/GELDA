#pragma once
enum class eInputStatus
{
	// for editor mode
	DEBUG_LEFT,
	DEBUG_RIGHT,
	DEBUG_UP,
	DEBUG_DOWN,
	DEBUG_ARROW_RELEASE,

	// for game mode
	LEFT,
	RIGHT,
	// UP,
	// DOWN,
	ARROW_RELEASE,
	JUMP_PRESS,
	JUMP_RELEASE
};

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