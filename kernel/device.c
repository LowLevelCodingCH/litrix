#include <litrix/device.h>
#include <litrix/memory.h>
#include <litrix/fs/lifs.h>
#include <litrix/stdout.h>

void init_dev(struct dev_t *dev, char *fname,
              enum dev_type type, enum dev_perms perms,
              void (*flush)(struct dev_t *)) {
    if(strlen(fname) > 128) {
        printf("[device] File name too long\n");
        return;
    }

    memcpy(dev->file, fname, strlen(fname));
    dev->perms = perms;
    dev->type = type;
    dev->flush = flush;
}

void read_dev(struct dev_t *dev, char *buf) {
    if(dev->perms == READ || dev->perms == READ_WRITE)
        lifs_read(dev->file, buf);
    else
        printf("[dev] Unallowed read from device: %s\n", dev->file);
}

void write_dev(struct dev_t *dev, unsigned int amount, char *buf) {
    if(dev->perms == WRITE || dev->perms == READ_WRITE)
        lifs_write(dev->file, amount, buf);
    else
        printf("[dev] Unallowed write to device: %s\n", dev->file);
}

void flush_dev(struct dev_t *dev) {
    dev->flush(dev);
}
