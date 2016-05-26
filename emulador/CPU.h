#pragma once
class CPU
{
private:
	unsigned short I=0; //
	unsigned short delay=0, soundTimer=0;
	unsigned short PC=0, SP=0;
	unsigned short stack[16];
	unsigned char V[16]; //registros
	unsigned char memoria[4096];
	unsigned short opcode=0;
	unsigned char n=0, kk=0, y=0, x=0;
	unsigned short nnn=0;
public:
	CPU();
	~CPU();
	void execute();
};

