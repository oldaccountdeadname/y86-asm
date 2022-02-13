#define MEMSIZE 1024    //1024 words of memory
#define MEMBYTE 4095

char get_byte(int address, bool *memError);
void set_byte(int address, unsigned char value, bool *memError);
void clear_mem(void);
int get_word(int address, bool *mem_err);
void set_word(int address, unsigned int value, bool *mem_err);
