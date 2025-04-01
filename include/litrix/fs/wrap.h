#define LIFS

#ifdef LIFS
int open(const char *name) {
    return lifs_open(name);
}

void close(int fd) {
    lifs_close(fd);
}

void write(int fd, int amount, char *buffer) {
    //TODO: Make it be an actual file when writing to tty

    if(fd == 1 || fd == 2) {
        print_len(buffer, amount);
        return;
    }

    lifs_fwrite(fd, amount, buffer);
}

void read(int fd, char *buffer) {
    lifs_fread(fd, buffer);
}

void creat(const char *name) {
    lifs_creat(name);
}

void fsctl(void) {
    lifs_ctl();
}
#endif

#ifdef SMFS
#endif
