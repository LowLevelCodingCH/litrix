unsigned char r_fs(void);
unsigned char r_gs(void);
void w_fs(unsigned char x);
void w_gs(unsigned char x);
unsigned int r_eax(void);
void w_eax(unsigned int x);
void jmp(unsigned int x);
void push(unsigned int x);
unsigned int cpuid(void);
