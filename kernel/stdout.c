#include <litrix/stdout.h>
#include <litrix/portio.h>
#include <litrix/memory.h>
#include <litrix/syscall.h>
#include <litrix/stdarg.h>

int cursor = 0;
unsigned char color = 0;

unsigned int strintlen(long int *s) {
    unsigned int i = 0;
  
    while(*s) {
        s++;
        i++;
    }

    s-=i;
    return i;
}

unsigned int strlen(const char *s) {
    unsigned int i = 0;
  
    while(*s) {
        s++;
        i++;
    }

    s-=i;
    return i;
}

unsigned char strcmp(const char *s1, const char *s2) {
    if(strlen(s1) != strlen(s2)) return 1;
    return memcmp((void*)s1, (void*)s2, strlen(s1));
}

void set_color(unsigned char c) {
    color = c;
}

void vis_cursor(void) {
    outb(0x3D4, 0x0E);
    outb(0x3D5, ((cursor/2) >> 8) & 0xFF);

    outb(0x3D4, 0x0F);
    outb(0x3D5, (cursor/2) & 0xFF);
}

void add_cursor(void) {
    cursor += 2;
    vis_cursor();
}

void sub_cursor(void) {
    cursor -= 2;
    vis_cursor();
}

void set_cursor(unsigned int i) {
    cursor = i * 2;
    vis_cursor();
}

//TODO: Trying to make this function not depend on any function. just the "cursor" var
//NOTE: This can be ported already. just change the outb to the appropriate.
//  Common options for port IO include
//    outportb
//    outpb
//    outb
//    portb
//NOTE: Name: memory mapped input output video graphics array put char
void mmio_vga_putc(unsigned char c) {
    if(c == 0 || c < 0x05)
        return;
    if(c == '\n') {
        int next_ln = ((cursor / 2) / 80) + 1;
        int current_ln = ((next_ln * 80) - 80) - 1;
        cursor = ((next_ln - 1) * 80) * 2;
        cursor -= 2;
        *(char*)(0xb8000+cursor) = ' ';
        cursor = (next_ln * 80) * 2;
        cursor -= 2;
    }
    else {
        *(char*)(0xb8000+cursor) = c;
        *(char*)(0xb8000+cursor+1) = color;
    }
    if(c == '\t') {
        *(char*)(0xb8000+cursor) = ' ';
        cursor += 2;
        *(char*)(0xb8000+cursor) = ' ';
        cursor += 2;
    }
    if(c == 0x05 && cursor >= 1) {
        *(char*)(0xb8000+cursor-2) = ' ';
        *(char*)(0xb8000+cursor) = ' ';
        cursor -= 4;
    }

    cursor += 2;

    outb(0x3D4, 0x0E);
    outb(0x3D5, ((cursor/2) >> 8) & 0xFF);
    outb(0x3D4, 0x0F);
    outb(0x3D5, (cursor/2) & 0xFF);
}

void pmap_io_serial_putc(unsigned char c) {
    outb(0x3F8, c);
}

void pmap_io_parallel_putc(unsigned char c) {
    outb(0x378, c);
}

void putlc(long int c) {
    *(long int*)(0xb8000 + cursor) = c;
    cursor += 1;
}

void putc(unsigned char c) {
    #ifdef VGA
    mmio_vga_putc(c);
    #endif

    #ifdef SERIAL
    pmap_io_serial_putc(c);
    #endif

    #ifdef PARALLEL
    pmap_io_parallel_putc(c);
    #endif
}

void putln(void) {
    putc('\n');
}

void print(char *s) {
    for(int i = 0; i < strlen(s); i++)
        putc(s[i]);
}

void putl(long int *s) {
    for(int i = 0; i < strintlen(s); i++)
        putlc(s[i]);
}

void puts(char *s) {
    for(int i = 0; i < strlen(s); i++)
        putc(s[i]);
}

void print_len(char *s, unsigned int n) {
    for(int i = 0; i < n; i++)
        putc(s[i]);
}

void println(char *s) {
    print(s);
    putln();
}

void clear(void) {
    set_cursor(0);
  
    for(int i = 0; i < 80*25; i++)
        putc(' ');

    set_cursor(0);
}

int atoi(char* str) {
    int num = 0;
    int sign = 1;
    int tc = 0;

    while (*str == ' ') {
        str++;
    }

    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }

    while (*str >= '0' && *str <= '9') {
        num = num * 10 + (*str - '0');
        str++;
        tc++;
    }

    if(tc < 1) return -1000;

    return sign * num;
}

char* itoa(int num, int base) {
    char buffer[16];
    char* str;
    int i = 0, j = 0;
    int is_negative = 0;

    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    do {
        if(base <= 10) buffer[i++] = (num % base) + '0';
        else if (base == 16){
            if((num % base) + '0' == ':') buffer[i++] = 'A';
            if((num % base) + '0' == ';') buffer[i++] = 'B';
            if((num % base) + '0' == '<') buffer[i++] = 'C';
            if((num % base) + '0' == '=') buffer[i++] = 'D';
            if((num % base) + '0' == '>') buffer[i++] = 'E';
            if((num % base) + '0' == '?') buffer[i++] = 'F';
        }
        num /= base;
    } while (num > 0);

    if (is_negative)
        buffer[i++] = '-';

    buffer[i] = '\0';

    for (j = 0; j < i; j++)
        str[j] = buffer[i - j - 1];
    str[j] = '\0';
    return str;
}

void printf(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    int i = 0;
    const char* p = fmt;
    while (*p) {
        if (*p == '%' && *(p++)) {
            switch (*p) {
            case 'f': {
                unsigned char f = (unsigned char)va_arg(args, unsigned int);
                set_color(f);
                i++;
                break;
            }
            case 'c': {
                char c = (char)va_arg(args, int);
                putc(c);
                i++;
                break;
            }
            case 'd': {
                unsigned int i = va_arg(args, int);
                puts(itoa(i, 10));
                i+=strlen(itoa(i, 10));
                break;
            }
            case 'x': {
                unsigned int i = va_arg(args, int);
                puts("0x");
                puts(itoa(i, 16));
                i+=strlen(itoa(i, 16));
                break;
            }
            case 'p': {
                unsigned int i = (unsigned int)va_arg(args, void*);
                puts("0x");
                puts(itoa(i, 16));
                i+=strlen(itoa(i, 16));
                break;
            }
            case 'o': {
                unsigned int i = va_arg(args, unsigned int);
                puts("0o");
                puts(itoa(i, 8));
                i+=strlen(itoa(i, 8));
                break;
            }
            case 's': {
                char* str = va_arg(args, char*);
                puts(str);
                i+=strlen(str);
                break;
            }
            default: {
                putc('%');
                putc(*p);
                i++;
                break;
            }
	    }
        } else {
            putc(*p);
            i++;
        }
        p++;
    }
    va_end(args);
}

void panic(char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    set_color(0x1f);
    clear();

    printf("Litrix Kernel Dump:\n");
    //PRINTF_BEGIN

    int i = 0;
    const char* p = fmt;
    while (*p) {
        if (*p == '%' && *(p++)) {
            switch (*p) {
            case 'f': {
                unsigned char f = (unsigned char)va_arg(args, unsigned int);
                set_color(f);
                i++;
                break;
            }
            case 'c': {
                char c = (char)va_arg(args, int);
                putc(c);
                i++;
                break;
            }
            case 'd': {
                unsigned int i = va_arg(args, int);
                puts(itoa(i, 10));
                i+=strlen(itoa(i, 10));
                break;
            }
            case 'x': {
                unsigned int i = va_arg(args, int);
                puts("0x");
                puts(itoa(i, 16));
                i+=strlen(itoa(i, 16));
                break;
            }
            case 'p': {
                unsigned int i = (unsigned int)va_arg(args, void*);
                puts("0x");
                puts(itoa(i, 16));
                i+=strlen(itoa(i, 16));
                break;
            }
            case 'o': {
                unsigned int i = va_arg(args, unsigned int);
                puts("0o");
                puts(itoa(i, 8));
                i+=strlen(itoa(i, 8));
                break;
            }
            case 's': {
                char* str = va_arg(args, char*);
                puts(str);
                i+=strlen(str);
                break;
            }
            default: {
                putc('%');
                putc(*p);
                i++;
                break;
            }
            }
        } else {
            putc(*p);
            i++;
        }
        p++;
    }

    //PRINTF_END

    set_cursor(1000000);
    for(;;) asm volatile("hlt");

    va_end(args);
}

void panerr(void) {
    printf("Error\n");
    return;
}
