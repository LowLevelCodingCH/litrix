enum dev_type {
    KEYBOARD,
    MOUSE,
    OUTPUT,
    VIDEO_CARD,
    BLOCK_ATA,
    OTHER,

    FILE_DESC,    // May be used kind-of but not really
    BLOCK_SSD,    // May be used in the future
    INPUT,        // Unused (use mouse and keyboard)
    PROCESS,      // Unused
};

enum dev_perms {
    NONE,
    WRITE,
    READ,
    READ_WRITE,
};

struct dev_t {
    char file[128];
    enum dev_type type;
    enum dev_perms perms;
    void (*flush)(struct dev_t *);
};

void init_dev(struct dev_t *dev, char *fname,
              enum dev_type type, enum dev_perms perms,
              void (*flush)(struct dev_t *));
void read_dev(struct dev_t *dev, char *buf);
void write_dev(struct dev_t *dev, unsigned int amount, char *buf);
void flush_dev(struct dev_t *dev);
