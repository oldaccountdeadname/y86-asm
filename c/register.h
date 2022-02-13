#define rax 0x0
#define rcx 0x1
#define rdx 0x2
#define rbx 0x3
#define rsp 0x4
#define rbp 0x5
#define rsi 0x6
#define rdi 0x7
#define r8 0x8
#define r9 0x9
#define r10 0xA
#define r11 0xB
#define r12 0xC
#define r13 0xD
#define r14 0xE
#define RNONE  0xf
#define REGSIZE 15
#define ZF     0x2
#define SF     0x1
#define OF     0x0

int getReg(int regNum);
void setReg(int regNum, unsigned int regValue);
void clearReg();
void clearCC();
void setCC(unsigned int bitNumber, unsigned int value);
int getCC(unsigned int bitNumber);