#include <litrix/device.h>
#include <litrix/memory.h>
#include <litrix/fs/lifs.h>
#include <litrix/fs/smfs.h>
#include <litrix/stdout.h>

void init_dev(struct dev_t *dev, char fname[32], enum dev_type type, enum dev_perms perms) {
    memcpy(dev->file, fname, 32);
    dev->perms = perms;
    dev->type = type;
}

void read_dev(struct dev_t *dev, char *buf) {
    if(dev->perms == READ || dev->perms == READ_WRITE)
        smfs_read(dev->file, buf);
    else
        printf(LITRIX_ERR "[io::dev] Unallowed read from device: %s\n", dev->file);
}

void write_dev(struct dev_t *dev, char *buf) {
    if(dev->perms == WRITE || dev->perms == READ_WRITE)
        smfs_write(dev->file, buf);
    else
        printf(LITRIX_ERR "[io::dev] Unallowed write to device: %s\n", dev->file);
}
