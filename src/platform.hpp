#pragma once
#include "raylib.h"
#include <cstdint>

class Platform {
  public:
	Texture2D texture;
	Platform();
	void ProcessInput(uint8_t* keys);
	void DrawUpdate(uint32_t* video_points, uint8_t* keys);
};
