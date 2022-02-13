#define MEMSIZE 1024    //1024 words of memory
#define MEMBYTE 4095
char getByte(int address, bool * memError);
void setByte(int address, unsigned char value, bool * memError);
void clearMem();
int getWord(int address, bool * memError);
void setWord(int address, unsigned int value, bool * memError);
