#define PARALLEL
#define VGA
#define DEBUG

extern int cursor;
extern unsigned char color;

void vis_cursor(void);
void add_cursor(void);
void sub_cursor(void);
void set_cursor(unsigned int i);

void set_color(unsigned char c);

unsigned int strlen(char *s);
unsigned char strcmp(char *s1, char *s2);

void mmio_vga_putc(unsigned char c);

void pmap_io_serial_putc(unsigned char c);
void pmap_io_parallel_putc(unsigned char c);

void putc(unsigned char c);
void putln(void);

void print(char *s);
void puts(char *s);
void print_len(char *s, unsigned int n);
void println(char *s);

void clear(void);

int atoi(char* str);
char* itoa(int num, int base);

void printf(char* fmt, ...);

void panic(char *fmt, ...);

#define LITRIX_LOG  "log  : "
#define LITRIX_WARN "warn : "
#define LITRIX_ERR  "err  : "
