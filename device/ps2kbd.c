#include <litrix/device.h>
#include <litrix/fs/lifs.h>
#include <litrix/portio.h>
#include <litrix/memory.h>
#include <litrix/stdout.h>
#include <litrix/device/ps2kbd.h>

void flush_ps2kbd(struct ps2kbd *kbd) {
    kbd->sent_strk = 0;
}

void read_ps2kbd(struct ps2kbd *kbd) {
    kbd->sent_strk = inb(0x60);
}

void write_ps2kbd(struct ps2kbd *kbd, char cmd) {
    write_dev(&kbd->dev, 1, &cmd);
    outb(0x64, cmd);
}

void init_ps2kbd(struct ps2kbd *kbd, char *fname) {
    struct dev_t d = {0};

    init_dev(&d, fname, KEYBOARD,
             READ_WRITE, (void*)NULL);

    kbd->dev.perms = d.perms;
    kbd->dev.type = d.type;
    kbd->dev.flush = d.flush;
    memcpy(kbd->dev.file, d.file, strlen(fname));
}
