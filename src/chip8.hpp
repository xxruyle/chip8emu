#pragma once
#include <cstdint>
#include <random>

class Chip8 {
  public:
	Chip8();

	uint8_t registers[16]{};
	uint8_t memory[16]{};
	uint16_t index{};
	uint16_t pc{}; // program counter
	uint16_t stack[16]{};
	uint8_t sp{}; // stack pointer
	uint8_t delay_timer{};
	uint8_t sound_timer{};
	uint8_t keypad[16]{};
	uint32_t video[64 * 32]{};
	uint16_t opcode;

	// rng
	std::default_random_engine rand_gen;
	std::uniform_int_distribution<uint8_t> rand_byte;

	void LoadROM(const char* file_name);

	// instructions
	void OP_00E0(); // CLS
	void OP_00EE(); // RET
	void OP_1nnn(); // JP addr
	void OP_2nnn(); // CALL addr
	void OP_3xkk(); // SE Vx, byte
	void OP_4xkk(); // SE Vx, byte
	void OP_5xy0(); // Se Vx, Vy
	void OP_6xkk(); // LD Vx, byte
	void OP_7xkk(); // ADD Vx, byte
	void OP_8xy0(); // LD Vx, Vy
	void OP_8xy1(); // OR Vx, Vy
	void OP_8xy2(); // AND Vx, Vy
	void OP_8xy3(); // XOR Vx, Vy
	void OP_8xy4(); // ADD Vx, Vy
	void OP_8xy5(); // SUB Vx, Vy
	void OP_8xy6(); // SHR Vx {, Vy}
	void OP_8xy7(); // SUBN Vx, Vy
	void OP_8xyE(); // SHL Vx {, Vy}
};
