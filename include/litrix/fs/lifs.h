#define LIFS_MAX_FILES 128
#define LIFS_BLOCKS 5
#define LIFS_RESV ((sizeof(char)*512)-((sizeof(int)*LIFS_BLOCKS)+sizeof(int)+(sizeof(char)*128)))

typedef unsigned int lba_t;

struct inode {
    lba_t blocks[LIFS_BLOCKS];
    unsigned int size;
    char name[128];
    char resv[LIFS_RESV];
};

extern struct inode lifs_inodes[LIFS_MAX_FILES];
extern lba_t biggest_adr;
extern lba_t last_biggest_adr;
extern int taken_inode_spaces;

// Control funcs
void lifs_ctl(void);

// INode funcs
void lifs_iread(struct inode *inod, char *buffer);
void lifs_blk_iwrite(struct inode *inod, char *buffer, unsigned int amount, unsigned int blkadr);
void lifs_iwrite(struct inode *inod, unsigned int amount, char *buffer);

// Name funcs
void lifs_read(char *name, char *buffer);
void lifs_write(char *name, unsigned int amount, char *buffer);
void lifs_creat(char *name);

// Other funcs
void lifs_list(void);
