#define PIT_FREQUENCY 1193182
#define PIT_CHANNEL_0 0x40
#define PIT_COMMAND   0x43

void init_pit(unsigned int frequency);
void pit_wait(unsigned int milliseconds);
