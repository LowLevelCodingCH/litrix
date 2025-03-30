#include <litrix/portio.h>
#include <litrix/keyboard.h>
#include <litrix/stdout.h>
#include <litrix/pit.h>

int caps_on;
int caps_lock;
char cchar;

unsigned char lowercase[256] = {
    1, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 225, '2', 0x05, '\t',
    'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', '[', ']', '\n', 0x02, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 1, '#', 'y', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0xFD, '*', 0x03, ' ',
    0x77, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFE, 'n', '7',
    0x77, 0x00, '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\n',
    0x77, 0x00, 0xFB, 0x00
};

unsigned char uppercase[256] = {
    1, 27, '!', '"', '$', '$', '%', '^', '&', '(', ')', '=', '?', '+', 0x05, '\t',
    'Q', 'W', 'E', 'R', 'T', 'Z', 'U', 'I', 'O', 'P', '{', '}', '\n', 0x02, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '@', '`', 1, '\'', 'Y', 'X', 'C', 'V',
    'B', 'N', 'M', ';', ':', '?', 0xFD, '*', 0x03, ' ',
    'C', 0XF1, 0XF2, 0XF3, 0XF4, 0XF5, 0XF6, 0XF7, 0XF8, 0XF9, 0XFA, 0xFE, 0xFE, '7',
    '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\n',
    'A', 'B', 0XFB, 0X00
};

void keyboard_handler(void) {
    static int shift_pressed = 0;
    char status = inb(KEYBOARD_STATUS);

    while((status & 0x01) == 0)
        status = inb(KEYBOARD_STATUS);

    unsigned char scancode = inb(KEYBOARD_DATA);

    if(scancode == 0x2A)
        shift_pressed = 1;
    else if(scancode == 0xAA)
        shift_pressed = 0;

    cchar = ((scancode & 0x80) ? 0 : (shift_pressed ? uppercase[scancode] : lowercase[scancode]));
}

void init_keyboard(void) {
    caps_on = 0;
    caps_lock = 0;
    cchar = 0;
}
