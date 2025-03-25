#define LIFS_MAX_FILES 1

typedef unsigned int lba_t;

struct inode {
    lba_t blocks[8];
    unsigned int size;
    char name[128];
    char resv[348];
};

extern struct inode lifs_inodes[LIFS_MAX_FILES];

void lifs_ctl(void);

// INode funcs
void lifs_iread(struct inode *inod, char buf[8][512]);

// Name funcs
void lifs_read(char name[128], char buf[8][512]);
