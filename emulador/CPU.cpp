#include "stdafx.h"
#include "CPU.h"
#include <iostream>
using namespace std;
void CPU::loadROM(const char * Path)
{
	V[watingKey >> 4] = watingKey & 0xF;
	state = true;
	watingKey = 0;
}
void CPU::interruption()
{

}
CPU::CPU()
{
	memoria[0x000] = 0x1A;
	memoria[0x001] = 0xBC;
	memoria[0xABC] = 0xAB;
}

CPU::~CPU()
{
}
void CPU::execute() {
	char teclaPresionada = 0xF;
	opcode = (memoria[PC] << 8) | (memoria[PC + 1]);
	n = (opcode & 0x000F);
	kk = (opcode & 0x00FF);
	nnn = (opcode & 0x0FFF);
	y = ((opcode & 0x00F0) >> 4);
	x = ((opcode & 0x0F00) >> 8);

	PC++;
	switch (opcode & 0xF000) {
	case 0x0000:
		switch (opcode & 0x00FF) {
			case 0x0E0:
				break;
				//LIMPIA LA PANTALLA
			case 0x0EE:
				PC = SP;
				SP--;
				break;
			default:
				break;
				//SALTA A UNA RUTINA DE LA MAQUINA EN nnn v0 = pc pc= sp ???
		}
	case 0x1000:
		PC = nnn;
		break;
	case 0x2000:
		SP++;
		PC = SP;
		break;
	case 0x3000:
		if (V[x] == kk) {
			PC += 2;
		}
		break;
	case 0x4000:
		if (V[x] != kk) {
			PC += 2;
		}
		break;
	case 0x5000:
		switch (opcode & 0x500F) {
		case 0x5000:
			if (V[x] == V[y]) {
				PC += 2;
			}
			break;
		}
	case 0x6000:
		V[x] = kk;
		break;
	case 0x7000:
		V[x] += kk;
		break;
	case 0x8000:
		switch (opcode & 0x800F) {
		case 0x8000:
			V[x] = V[y];
			break;
		case 0x8001:
			V[x] = V[x] | V[y];
			break;
		case 0x8002:
			V[x] = V[x] & V[y];
			break;
		case 0x8003:
			V[x] = V[x] ^ V[y];
			break;
		case 0x8004:
			V[x] = V[x] + V[y];
			if (V[x] < V[y]) {
				V[y] = 1;
			}
			else {
				V[y] = 0;
			}
			break;
		case 0x8005:
			break;
			//preguntar a pipe esta raro
		case 0x8006:
			if (V[x] & 0x01) {
				V[0xF] = 0x01;
			}
			else {
				V[0xF] = 0x00;
			}
			V[x] = (V[x] >> 1);
			break;
		case 0x8007:
			break;
			//preguntar a pipe esta raro
		case 0x800E:
			if (V[x] & 0x80) {  // V[x] & (1<<7)
				V[0xF] = 0x01;
			}
			V[x] = (V[x] << 1);
			break;
		}
	case 0x9000:
		if (V[x] != V[y]) {
			PC += 2;
		}
		break;
	case 0xA000:
		I = nnn;
	case 0xB000:
		PC = nnn + V[0];
	case 0xC000:
		V[x] = (rand() % (0xFF + 1))& kk;

	case 0xD000:
		// aun no
	case 0xE000:
		switch (opcode & 0xE0FF) {
		case 0xE09E:
			// configurar tecla presionada
			
			if (teclaPresionada == V[x]) {
				PC += 2;
			}
			break;
		case 0xE0A1:
			// configurar tecla presionada
			teclaPresionada = 0xF;
			if (teclaPresionada != V[x]) {
				PC += 2;
			}
			break;
		}
	case 0xF000:
		switch (opcode & 0xF0FF) {
		case 0xF007:
			V[x] = delay;
			break;
		case 0xF00A:
			while (true) {
				//esperamos hasta que una tecla sea presionada
				teclaPresionada = 0xF;
				V[x] = teclaPresionada;
				break;
			}
			break;
		case 0xF015:
			delay = V[x];
			break;
		case 0xF018:
			soundTimer = V[x];
			break;
		case 0xF01E:
			I += V[x];
			break;
		case 0xF029:
			break;
			//ni idea
		case 0xF033:
			memoria[I] = V[x] / 100;
			memoria[I + 1] = (V[x] / 10) % 10;
			memoria[I + 2] = V[x] % 10;
			break;
		case 0xF055:
			for (int i = 0; i <= x; i++) {
				memoria[I + i] = V[i];
			}
			break;
		case 0xF065:
			for (int i = 0; i <= x; i++) {
				 V[i] = memoria[I + i];
			}
			break;
		}
	}

}