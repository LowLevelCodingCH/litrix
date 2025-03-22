#define KEYBOARD_STATUS 0x64
#define KEYBOARD_DATA 0x60

extern char cchar;

void init_keyboard(unsigned char silent);
void keyboard_handler(void);
