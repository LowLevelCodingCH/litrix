int open(const char *name);
void close(int fd);
void write(int fd, int amount, char *buffer);
void read(int fd, char *buffer);
void creat(const char *name);
void fsctl(void);
