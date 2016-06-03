#include "stdafx.h"
#include "CPU.h"
#include <fstream>

using namespace std;
void CPU::loadROM(const char * path)
{
	//V[waitingKey >> 4] = waitingKey & 0xF;
	state = true;
	waitingKey = 0;
	unsigned short p = 0x200;
	for (ifstream file(path, std::ios::binary); file.good();) {
		memoria[p++] = file.get();
	}
}
void CPU::interruption()
{
	V[waitingKey >> 4] = waitingKey & 0xF;
	state = false;
	waitingKey = 0;

}
CPU::CPU()
{
	memset(stack, 0, sizeof(stack));
	memset(memoria, 0, sizeof(memoria));
	memset(keys, 0, sizeof(keys));
	memset(V, 0, sizeof(V));

	unsigned char fuente[16 * 5] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0,	// 0
		0x20, 0x60, 0x20, 0x20, 0x70,	// 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0,	// 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0,	// 3
		0x90, 0x90, 0xF0, 0x10, 0x10,	// 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0,	// 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0,	// 6
		0xF0, 0x10, 0x20, 0x40, 0x40,	// 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0,	// 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0,	// 9
		0xF0, 0x90, 0xF0, 0x90, 0x90,	// A
		0xE0, 0x90, 0xE0, 0x90, 0xE0,	// B
		0xF0, 0x80, 0x80, 0x80, 0xF0,	// C
		0xE0, 0x90, 0x90, 0x90, 0xE0,	// D
		0xF0, 0x80, 0xF0, 0x80, 0xF0,	// E
		0xF0, 0x80, 0xF0, 0x80, 0x80 };	// F

	for (int i = 0; i < 16 * 5; i++) {
		memoria[i] = fuente[i];
	}

	I = 0; //
	delay = 0;
	soundTimer = 0;
	PC = 0x200;
	SP = 0;
	opcode = 0;
	n = 0, kk = 0, y = 0, x = 0;
	nnn = 0;
	state = true;
	waitingKey = 0;

	
	memset(estadosVisitados, 0, sizeof(estadosVisitados));

}

CPU::~CPU()
{
}
void CPU::execute() {
	
	if (state == false)return;
	opcode = (memoria[PC] << 8) | (memoria[PC + 1]);
	n = (opcode & 0x000F);
	kk = (opcode & 0x00FF);
	nnn = (opcode & 0x0FFF);
	y = ((opcode & 0x00F0) >> 4);
	x = ((opcode & 0x0F00) >> 8);
	PC+=2;
	for (int i = 0; i < 500000; i++) {

	}
	switch (opcode & 0xF000) {
	case 0x0000:

		switch (opcode & 0x00FF) {
		case 0x0E0:
			estadosVisitados[0] = true;
			memset(gpu.vram, 0, sizeof(gpu.vram));


			//V[0xF] = 1;
			gpu.Render();
			break;

		case 0x0EE:
			estadosVisitados[1] = true;
			PC = stack[--SP];

			//PC += 2;
			break;
		default:
			estadosVisitados[2] = true;
			break;
			//SALTA A UNA RUTINA DE LA MAQUINA EN nnn v0 = pc pc= sp ???
		}
		break;
	case 0x1000:
		estadosVisitados[3] = true;
		PC = nnn;
		break;
	case 0x2000:
		estadosVisitados[4] = true;
		stack[SP] = PC;
		SP++;

		PC = nnn;
		break;
	case 0x3000:
		estadosVisitados[5] = true;
		if (V[x] == kk) {
			PC += 2;
		}
		break;
	case 0x4000:
		estadosVisitados[6] = true;
		if (V[x] != kk) {
			PC += 2;
		}
		break;
	case 0x5000:
		estadosVisitados[7] = true;
		switch (opcode & 0x500F) {
		case 0x5000:
			if (V[x] == V[y]) {
				PC += 2;
			}
			break;
		}
		break;
	case 0x6000:
		estadosVisitados[8] = true;
		V[x] = kk;
		break;
	case 0x7000:
		estadosVisitados[9] = true;
		V[x] += kk;
		break;
	case 0x8000:

		switch (opcode & 0x800F) {
		case 0x8000:
			estadosVisitados[10] = true;
			V[x] = V[y];
			break;
		case 0x8001:
			estadosVisitados[11] = true;
			V[x] |= V[y];
			break;
		case 0x8002:
			estadosVisitados[12] = true;
			V[x] &= V[y];
			break;
		case 0x8003:
			estadosVisitados[13] = true;
			V[x] ^= V[y];
			break;
		case 0x8004:
			estadosVisitados[14] = true;
			V[x] = V[x] + V[y];
			if (V[x] < V[y]) {
				V[0xF] = 1;
			}
			else {
				V[0xF] = 0;
			}

			break;
		case 0x8005:
			estadosVisitados[15] = true;
			if (V[x] < V[y])
			{
				V[0xF] = 1;
			}
			else
			{
				V[0xF] = 0;
			}
			V[x] -= V[y];
			break;
		case 0x8006:
			estadosVisitados[16] = true;
			V[0xF] = V[x] & 0x01;
			V[x] = (V[x] >> 1);
			break;
		case 0x8007:
			estadosVisitados[17] = true;

			if (V[y] < V[x])
			{
				V[0xF] = 1;
			}
			else
			{
				V[0xF] = 0;
			}
			V[x] = V[y] - V[x];
			break;
		case 0x800E:
			estadosVisitados[18] = true;
			if (V[x] & 0x80) {  // V[x] & (1<<7)
				V[0xF] = 0x01;
			}
			else {
				V[0xF] = 0x00;
			}
			V[x] = (V[x] << 1);
			break;
		}
		break;
	case 0x9000:
		estadosVisitados[19] = true;
		if (V[x] != V[y]) {
			PC += 2;
		}
		break;
	case 0xA000:
		estadosVisitados[20] = true;
		I = nnn;
		break;
	case 0xB000:
		estadosVisitados[21] = true;
		PC = nnn + V[0];
		break;
	case 0xC000:
		estadosVisitados[22] = true;
		V[x] = (rand() % 0xFF)& kk;
		break;
	case 0xD000:
		estadosVisitados[23] = true;
		V[0xF] = 0;

		for (int yline = 0; yline < n; yline++)
		{
			unsigned char pixel = memoria[I + yline];
			for (int xline = 0; xline < 8; xline++)
			{
				int horizontal = (V[x] + xline);
				int vertical = (((V[y] + yline))) * 64;
				if ((pixel &(0x80 >> xline)) != (gpu.vram[V[x] + V[y]]))
				{
					gpu.vram[horizontal + vertical] ^= (pixel &(0x80 >> xline));
					V[0xF] = 1;
					
				}
				//else {
				//	V[0xF] = 0;
				//}
			}
		}


		if (V[0xF] == 1)
			gpu.Render();
		break;
	case 0xE000:
		switch (opcode & 0xE0FF) {
		case 0xE09E:
			estadosVisitados[24] = true;
			if (keys[V[x]]) {
				PC += 2;
			}
			break;
		case 0xE0A1:
			estadosVisitados[25] = true;
			if (!keys[V[x]]) {
				PC += 2;
			}
			break;
		}
		break;
	case 0xF000:

		switch (opcode & 0xF0FF) {
		case 0xF007:
			estadosVisitados[26] = true;
			V[x] = delay;
			break;
		case 0xF00A:
			estadosVisitados[27] = true;
			state = false;
			waitingKey = waitingKey & 0x0F;
			break;
		case 0xF015:
			estadosVisitados[28] = true;
			delay = V[x];
			break;
		case 0xF018:
			estadosVisitados[29] = true;
			soundTimer = V[x];
			break;
		case 0xF01E:
			estadosVisitados[30] = true;
			I += V[x];

			break;
		case 0xF029:
			estadosVisitados[31] = true;
			I = V[x] * 5;
			break;
			//ni idea
		case 0xF033:
			estadosVisitados[32] = true;
			memoria[I] = V[x] / 100;
			memoria[I + 1] = (V[x] / 10) % 10;
			memoria[I + 2] = (V[x] % 100) % 10;
			break;
		case 0xF055:
			estadosVisitados[33] = true;
			for (int i = 0; i <= x; i++) {
				memoria[I + i] = V[i];
			}
			break;
		case 0xF065:
			estadosVisitados[34] = true;
			for (int i = 0; i <= x; i++) {
				V[i] = memoria[I + i];
			}
			break;
		}
	}
	if (delay > 0) {
		delay--;
	}
	if (soundTimer > 0) {
		soundTimer--;
	}
}