#include <litrix/pit.h>
#include <litrix/stdout.h>
#include <litrix/portio.h>

void init_pit(unsigned int frequency) {
    unsigned int divisor = PIT_FREQUENCY / frequency;

    outb(PIT_COMMAND, 0x36);
    outb(PIT_CHANNEL_0, (unsigned char)(divisor & 0xFF));
    outb(PIT_CHANNEL_0, (unsigned char)((divisor >> 8) & 0xFF));

    printf(LITRIX_LOG "[io::pit] Initialized PIT");
}

void pit_wait(unsigned int milliseconds) {
    unsigned int count = 11932 * milliseconds;

    outb(count & 0xFF, PIT_CHANNEL_0);
    outb((count >> 8) & 0xFF, PIT_CHANNEL_0);

    while (count > 0) {
        count--;
    }
}
