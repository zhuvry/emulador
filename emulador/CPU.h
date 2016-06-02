#pragma once
#include "GPU.h"
class CPU
{
private:
	uint16_t I=0; //
	uint8_t delay=0, soundTimer=0;
	uint16_t PC = 0x200;
	uint8_t SP = 0;
	uint16_t stack[16];
	uint8_t V[16]; //registros
	uint8_t memoria[0x1000];
	uint16_t opcode=0;
	uint8_t n=0, kk=0, y=0, x=0;
	uint16_t nnn=0;
	//GPU gpu;
public:
	GPU gpu;
	bool estadosVisitados[35];
	bool state = true;
	void loadROM(const char *Path);
	uint8_t keys[16];
	uint8_t waitingKey;
	void interruption();
	CPU();
	~CPU();
	void execute();
};

