#pragma once
enum class eInputStatus {
  // for game mode
  LEFT,
  RIGHT,
  UP,
  DOWN,
  ARROW_RELEASE,
  JUMP_PRESS,
  JUMP_RELEASE
};

enum class eLifeStatus { ALIVE, DEAD };

enum class eDirectionStatus { LEFT, RIGHT };

enum class eMoveStatus { STOP, MOVE };

enum class eJumpStatus { NO_JUMP, JUMP, JUMP_DECELERATION, FALL };