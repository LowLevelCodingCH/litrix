#define SMFS__MAX_FILES 100

struct smfs_entr {
    char name[32];
    char data[480];
} __attribute__((packed));

extern struct smfs_entr fs_entries[SMFS__MAX_FILES];

void smfs_ctl(void);
int smfs_read(char name[32], char *buf);
int smfs_write(char name[32], char data[512-32]);
int smfs_rename(char name[32], char newname[32]);
int smfs_creat(char name[32]);
void smfs_list(void);
int smfs_test(char name[32]);
