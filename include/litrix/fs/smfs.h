#define SMFS__MAX_FILES 100

struct smfs_entr {
    char name[32];
    char data[480];
};

extern struct smfs_entr fs_entries[SMFS__MAX_FILES];

void smfs_ctl(void);
void smfs_print_content(char name[32]);
void smfs_write_content(char name[32], char data[512-32]);
