#define KEYBOARD_STATUS 0x64
#define KEYBOARD_DATA 0x60

extern char cchar;

extern unsigned char uppercase[256];
extern unsigned char lowercase[256];

void init_keyboard(void);
void keyboard_handler(void);
