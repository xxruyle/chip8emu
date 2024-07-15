#include "platform.hpp"
#include "iostream"
#include "raylib.h"
#include <cstdint>

Platform::Platform() {}

/* ====================
 * keyboard key mapping
 * ====================
 *
 *  1 2 3 C  -> 1 2 3 4
 *  4 5 6 D  -> Q W E R
 *  7 8 9 E  -> A S D F
 *  A 0 B F  -> Z X C V
 *
 */
void Platform::ProcessInput(uint8_t* keys)
{
	if (IsKeyDown(KEY_ONE)) {
		keys[0x1] = 1;
	}
	else if (IsKeyUp(KEY_ONE)) {
		keys[0x1] = 0;
	}
	if (IsKeyDown(KEY_TWO)) {
		keys[0x2] = 1;
	}
	else if (IsKeyUp(KEY_TWO)) {
		keys[0x2] = 0;
	}
	if (IsKeyDown(KEY_THREE)) {
		keys[0x3] = 1;
	}
	else if (IsKeyUp(KEY_THREE)) {
		keys[0x3] = 0;
	}
	if (IsKeyDown(KEY_FOUR)) {
		keys[0xC] = 1;
	}
	else if (IsKeyUp(KEY_FOUR)) {
		keys[0xC] = 0;
	}
	if (IsKeyDown(KEY_Q)) {
		keys[0x4] = 1;
	}
	else if (IsKeyUp(KEY_Q)) {
		keys[0x4] = 0;
	}

	if (IsKeyDown(KEY_W)) {
		keys[0x5] = 1;
	}
	else if (IsKeyUp(KEY_W)) {
		keys[0x5] = 0;
	}

	if (IsKeyDown(KEY_E)) {
		keys[0x6] = 1;
	}
	else if (IsKeyUp(KEY_E)) {
		keys[0x6] = 0;
	}

	if (IsKeyDown(KEY_R)) {
		keys[0xD] = 1;
	}
	else if (IsKeyUp(KEY_R)) {
		keys[0xD] = 0;
	}

	if (IsKeyDown(KEY_A)) {
		keys[0x7] = 1;
	}
	else if (IsKeyUp(KEY_A)) {
		keys[0x7] = 0;
	}

	if (IsKeyDown(KEY_S)) {
		keys[0x8] = 1;
	}
	else if (IsKeyUp(KEY_S)) {
		keys[0x8] = 0;
	}

	if (IsKeyDown(KEY_D)) {
		keys[0x9] = 1;
	}
	else if (IsKeyUp(KEY_D)) {
		keys[0x9] = 0;
	}

	if (IsKeyDown(KEY_F)) {
		keys[0xE] = 1;
	}
	else if (IsKeyUp(KEY_F)) {
		keys[0xE] = 0;
	}

	if (IsKeyDown(KEY_Z)) {
		keys[0xA] = 1;
	}
	else if (IsKeyUp(KEY_Z)) {
		keys[0xA] = 0;
	}

	if (IsKeyDown(KEY_X)) {
		keys[0x0] = 1;
	}
	else if (IsKeyUp(KEY_X)) {
		keys[0x0] = 0;
	}

	if (IsKeyDown(KEY_C)) {
		keys[0xB] = 1;
	}
	else if (IsKeyUp(KEY_C)) {
		keys[0xB] = 0;
	}

	if (IsKeyDown(KEY_V)) {
		keys[0xF] = 1;
	}
	else if (IsKeyUp(KEY_V)) {
		keys[0xF] = 0;
	}
}

void Platform::DrawUpdate(uint32_t* video_points, uint8_t* keys)
{
	int drawing_scale = 10;
	BeginDrawing();
	ClearBackground(RAYWHITE);
	for (uint32_t row = 0; row < 32; row++) {
		for (uint32_t col = 0; col < 64; col++) {
			uint32_t index = row * 64 + col;
			if (video_points[index]) {
				DrawRectangle(col * drawing_scale, row * drawing_scale, drawing_scale, drawing_scale, BLACK);
			}
			else {
				DrawRectangle(col * drawing_scale, row * drawing_scale, drawing_scale, drawing_scale, LIGHTGRAY);
			}
		}
	}
	EndDrawing();
}
