struct ps2kbd {
    struct dev_t dev;
    char sent_strk;
};

void flush_ps2kbd(struct ps2kbd *kbd);
void read_ps2kbd(struct ps2kbd *kbd);
void write_ps2kbd(struct ps2kbd *kbd, char cmd);
void init_ps2kbd(struct ps2kbd *kbd, char *fname);
