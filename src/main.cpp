#include "chip8.hpp"
#include "platform.hpp"
#include "raylib.h"
#include <chrono>
#include <iostream>

int main(int argc, char** argv)
{
	if (argc != 4) {
		std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n";
		std::exit(EXIT_FAILURE);
	}

	int video_scale = std::stoi(argv[1]);
	int cycle_delay = std::stoi(argv[2]);
	char const* rom_filename = argv[3];

	Chip8 cpu;
	Platform platform;

	cpu.LoadROM(rom_filename);
	int video_pitch = sizeof(cpu.video[0]) * VIDEO_WIDTH;
	auto last_cycle_time = std::chrono::high_resolution_clock::now();

	const int screen_width = 800;
	const int screen_height = 450;
	InitWindow(screen_width, screen_height, rom_filename);
	while (!WindowShouldClose()) {
		platform.ProcessInput(cpu.keypad);
		auto current_time = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(current_time - last_cycle_time).count();

		if (dt > cycle_delay) {
			last_cycle_time = current_time;
			cpu.Cycle();
			platform.DrawUpdate(cpu.video, cpu.keypad);
		}
	}
	return 0;
}
