#include "chip8.hpp"
#include <chrono>
#include <cstring>
#include <fstream>
#include <random>

const unsigned int FONTSET_SIZE = 80;
uint8_t fontset[FONTSET_SIZE] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

const unsigned int START_ADDRESS = 0x200;
const unsigned int FONTSET_START_ADDRESS = 0x50;

// load ROM into memory
void Chip8::LoadROM(const char* file_name)
{
	std::ifstream file(file_name, std::ios::binary | std::ios::ate);

	if (file.is_open()) {
		std::streampos size = file.tellg();
		char* buffer = new char[size];

		file.seekg(0, std::ios::beg);
		file.read(buffer, size);
		file.close();

		for (long i = 0; i < size; ++i) {
			memory[START_ADDRESS + i] = buffer[i];
		}

		// free temp buffer
		delete[] buffer;
	}
}

Chip8::Chip8() : rand_gen(std::chrono::system_clock::now().time_since_epoch().count())
{
	// set program counter
	pc = START_ADDRESS;

	// load fonts
	for (long i = 0; i < FONTSET_SIZE; ++i) {
		memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}

	// set rng random byte
	rand_byte = std::uniform_int_distribution<uint8_t>(0, 255U);
}

// -- INSTRUCTIONS --

// clear video display
void Chip8::OP_00E0() { memset(video, 0, sizeof(video)); }

// return from a subroutine
void Chip8::OP_00EE()
{
	sp--;
	pc = stack[sp];
}

// jump to location nnn
// interpreter sets program counter to nnn
void Chip8::OP_1nnn()
{
	uint16_t address = opcode & 0x0FFFu;
	pc = address;
}

// call subroutine at nnn
void Chip8::OP_2nnn()
{

	uint16_t address = opcode & 0x0FFFu;
	stack[sp] = pc;
	sp++;
	pc = address;
}

// skip next instruction if Vx = kk
// The interpreter compares register Vx to kk, increments the program counter by 2 if equal.
void Chip8::OP_3xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u; // getting Vx
	uint8_t byte = opcode & 0x00FFu;	   // getting kk byte

	if (registers[Vx] == byte) {
		pc += 2;
	}
}

// Skip next instruction if Vx != kk.
void Chip8::OP_4xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u; // getting Vx
	uint8_t byte = opcode & 0x00FFu;	   // getting kk byte

	if (registers[Vx] != byte) {
		pc += 2;
	}
}

// skip next instruction if Vx = Vy
void Chip8::OP_5xy0()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u; // getting Vx
	uint8_t Vy = (opcode & 0x00F0u) >> 4u; // getting Vy

	if (registers[Vx] == registers[Vy]) {
		pc += 2;
	}
}

// Set Vx = kk
// The interpreter puts the value kk into register Vx.
void Chip8::OP_6xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u; // getting Vx
	uint8_t byte = opcode & 0x00FFu;	   // getting kk byte

	registers[Vx] = byte;
}

// Set Vx = Vx + kk.
void Chip8::OP_7xkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u; // getting Vx
	uint8_t byte = opcode & 0x00FFu;	   // getting kk byte

	registers[Vx] += byte;
}

// Set Vx = Vy
void Chip8::OP_8xy0()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u; // getting Vx
	uint8_t Vy = (opcode & 0x00F0u) >> 4u; // getting Vy

	registers[Vx] = registers[Vy];
}

// Set Vx |= Vy
void Chip8::OP_8xy1()
{

	uint8_t Vx = (opcode & 0x0F00u) >> 8u; // getting Vx
	uint8_t Vy = (opcode & 0x00F0u) >> 4u; // getting Vy
	registers[Vx] |= registers[Vy];
}

// Set Vx = Vx AND Vy.
void Chip8::OP_8xy2()
{

	uint8_t Vx = (opcode & 0x0F00u) >> 8u; // getting Vx
	uint8_t Vy = (opcode & 0x00F0u) >> 4u; // getting Vy
	registers[Vx] &= registers[Vy];
}

// Set Vx = Vx XOR Vy.
void Chip8::OP_8xy3()
{

	uint8_t Vx = (opcode & 0x0F00u) >> 8u; // getting Vx
	uint8_t Vy = (opcode & 0x00F0u) >> 4u; // getting Vy
	registers[Vx] ^= registers[Vy];
}

// Set Vx = Vx + Vy, set VF = carry.
void Chip8::OP_8xy4()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u; // getting Vx
	uint8_t Vy = (opcode & 0x00F0u) >> 4u; // getting Vy

	uint16_t sum = registers[Vx] + registers[Vy];

	// set VF (flag) to 1 or 0
	if (sum > 255u) {
		registers[0xF] = 1;
	}
	else {
		registers[0xF] = 0;
	}

	registers[Vx] = sum & 0xFFu;
}

// Set Vx = Vx - Vy, set VF = NOT borrow.
// If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
void Chip8::OP_8xy5()
{

	uint8_t Vx = (opcode & 0x0F00u) >> 8u; // getting Vx
	uint8_t Vy = (opcode & 0x00F0u) >> 4u; // getting Vy

	// set VF (flag) to 1 or 0
	if (registers[Vx] > registers[Vy]) {
		registers[0xF] = 1;
	}
	else {
		registers[0xF] = 0;
	}

	registers[Vx] -= registers[Vy];
}

// Set Vx = Vx SHR 1.
// If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
void Chip8::OP_8xy6()
{

	uint8_t Vx = (opcode & 0x0F00u) >> 8u; // getting Vx

	registers[0xF] = (registers[Vx] & 0x1u);

	registers[Vx] >>= 1; // divide by 2
}

// Set Vx = Vy - Vx, set VF = NOT borrow.
// If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
void Chip8::OP_8xy7()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u; // getting Vx
	uint8_t Vy = (opcode & 0x00F0u) >> 4u; // getting Vy

	// set VF (flag) to 1 or 0
	if (registers[Vy] > registers[Vx]) {
		registers[0xF] = 1;
	}
	else {
		registers[0xF] = 0;
	}

	registers[Vx] = registers[Vy] - registers[Vx];
}

// Set Vx = Vx SHL 1.
// If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
void Chip8::OP_8xyE()
{

	uint8_t Vx = (opcode & 0x0F00u) >> 8u; // getting Vx

	registers[0xF] = (registers[Vx] & 0x80u) >> 7u;
	registers[Vx] <<= 1;
}
