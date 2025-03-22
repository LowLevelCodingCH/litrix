#include <litrix/stdout.h>
#include <litrix/portio.h>
#include <litrix/memory.h>
#include <litrix/stdarg.h>

int cursor = 0;
unsigned char color  = 0x07;

unsigned int strlen(char *s) {
    unsigned int i = 0;
  
    while(*s != 0) {
        s++;
        i++;
    }

    s-=i;
    return i;
}

unsigned char strcmp(char *s1, char *s2) {
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
    cursor = i*2;
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
void mmap_io_vga_putc(unsigned char c) {
    if(c == 0)
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
        cursor -= 2*(2);
    }
    cursor += 2;

    outb(0x3D4, 0x0E);
    outb(0x3D5, ((cursor/2) >> 8) & 0xFF);
    outb(0x3D4, 0x0F);
    outb(0x3D5, (cursor/2) & 0xFF);
}

void putc(unsigned char c) {
    mmap_io_vga_putc(c);
}

void putln(void) {
    putc('\n');
}

void print(char *s) {
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
    set_color(0x07);

    set_cursor(0);
  
    for(int i = 0; i < 80*25; i++)
        putc(' ');

    set_cursor(0);

    set_color(0x07);
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

char* itoa(int num) {
    char buffer[16];
    char* str;
    int i = 0, j = 0;
    int is_negative = 0;

    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    do {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    } while (num > 0);

    if (is_negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    for (j = 0; j < i; j++) {
        str[j] = buffer[i - j - 1];
    }
    str[j] = '\0';
    return str;
}

void printf(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    int i = 0;
    const char* p = fmt;
    while (*p) {
        if (*p == '%' && *(p + 1)) {
            p++;
            switch (*p) {
            case 'f': {
                unsigned char f = (unsigned char)va_arg(args, char);
                set_color(f);
            }
            case 'c': {
                char c = (char)va_arg(args, int);
                putc(c);
                i++;
                break;
            }
            case 'd': {
                unsigned int i = va_arg(args, int);
                print(itoa(i));
                i+=strlen(itoa(i));
                break;
            }
            case 's': {
                char* str = va_arg(args, char*);
                print(str);
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
