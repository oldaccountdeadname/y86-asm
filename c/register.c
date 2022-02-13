#include <stdio.h>
#include <memory.h>
#include "memory.h"
#include "tools.h"
#include "register.h"

static int registers[REGSIZE];
static unsigned int cc;

void setCC(unsigned int bitNumber, unsigned int value)
{
    if (bitNumber == OF || bitNumber == SF || bitNumber == ZF) 
    {
        cc = assignBit(bitNumber, value, cc);
    }
}

int getCC(unsigned int bitNumber) 
{
    if (bitNumber == OF || bitNumber == SF || bitNumber == ZF) 
    {
        return getBits(bitNumber, bitNumber, cc);
    }
}
void clearReg() 
{
    memset(registers, 0, REGSIZE);
}

void clearCC() 
{
    setCC(OF, 0);
    setCC(ZF, 0);
    setCC(SF, 0);
}

int getReg(int regNum) 
{
    if (regNum >= 0 && regNum < REGSIZE) 
    {
        return registers[regNum];
    }
}

void setReg(int regNum, unsigned int regValue) 
{
    if (regNum >= 0 && regNum < REGSIZE) 
    {
        registers[regNum] = regValue;
    }
}