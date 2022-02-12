#ifndef REGISTER_H
#define REGISTER_H

enum reg {
	RAX = 0x0,
	RCX,
	RDX,
	RBX,
	RSP,
	RSI,
	RDI,
	R8,
	R9,
	R10,
	R11,
	R12,
	R13,
	R14,
	RNONE,
};

enum flags {
	OF = 0x0,
	SF,
	ZF,
};

int getReg(int regNum);
void storeReg(int regNum, int value);
void clearRegister();
void clearCC();
void setCC(int ccNum, int value);

#endif
