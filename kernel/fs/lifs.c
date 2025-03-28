#include <litrix/stdout.h>
#include <litrix/memory.h>
#include <litrix/disk.h>
#include <litrix/fs/lifs.h>

/*
 * Litrix FS
 *        File System
 */

struct inode lifs_inodes[LIFS_MAX_FILES] = {0};
lba_t biggest_adr = LIFS_MAX_FILES * LIFS_BLOCKS;
lba_t last_biggest_adr = LIFS_MAX_FILES * LIFS_BLOCKS;
int taken_inode_spaces = 1;

void lifs_ctl(void) {
    biggest_adr = LIFS_MAX_FILES * LIFS_BLOCKS;
    last_biggest_adr = LIFS_MAX_FILES * LIFS_BLOCKS;
    char buf[512] = {0};

    for(int i = 0; i < LIFS_MAX_FILES; i++) {
        memset(buf, 0, 512);

        ata_read_sector(i, buf);

        lifs_inodes[i].size = ((unsigned int*)(buf))[LIFS_BLOCKS];

	if(lifs_inodes[i].size != 0) {
            for(int j = 0; j < LIFS_BLOCKS; j++) {
                lifs_inodes[i].blocks[j] = ((lba_t*)(buf))[j];
                if(last_biggest_adr < lifs_inodes[i].blocks[j]) {
                    biggest_adr = lifs_inodes[i].blocks[j];
                    last_biggest_adr = lifs_inodes[i].blocks[j];
                }
            }

            if(lifs_inodes[i].blocks[0] != 0) taken_inode_spaces++;

            for(int j = 0; j < 128; j++)
                lifs_inodes[i].name[j] = buf[4 * LIFS_BLOCKS + 4 + j];

            memset(lifs_inodes[i].resv, 0, LIFS_RESV);
        }
    }
}

void lifs_creat(char *name) {
    lba_t block0 = 0;
    lba_t block1 = 0;
    lba_t block2 = 0;
    lba_t block3 = 0;
    lba_t block4 = 0;

    char nam[128];
    memset(nam, 0, 128);

    block0 = biggest_adr + 1;
    block1 = biggest_adr + 2;
    block2 = biggest_adr + 3;
    block3 = biggest_adr + 4;
    block4 = biggest_adr + 5;

    char blockdat[512] = {0};
    struct inode inod = {0};

    inod.blocks[0] = block0;
    inod.blocks[1] = block1;
    inod.blocks[2] = block2;
    inod.blocks[3] = block3;
    inod.blocks[4] = block4;
    inod.size = 512*LIFS_BLOCKS;

    if(strlen(name) <= 128) {
        for(int i = 0; i < strlen(name); i++)
            nam[i] = name[i];
    } else {
        printf("[lifs] File name too long\n");
        return;
    }

    ((int*)(blockdat))[0] = inod.blocks[0];
    ((int*)(blockdat))[1] = inod.blocks[1];
    ((int*)(blockdat))[2] = inod.blocks[2];
    ((int*)(blockdat))[3] = inod.blocks[3];
    ((int*)(blockdat))[4] = inod.blocks[4];
    ((int*)(blockdat))[LIFS_BLOCKS] = inod.size;

    for(int i = 0; i < 128; i++)
        blockdat[i + (LIFS_BLOCKS * 4) + (1 * 4)] = nam[i];

    ata_write_sector(taken_inode_spaces - 1, blockdat);

    lifs_ctl();
}

void lifs_iread(struct inode *inod, char *buffer) {
    if(!inod) {
        printf("[lifs] No valid inode lifs_iread(%x, %x);\n", (unsigned int)inod, (unsigned int)buffer);
        return;
    }

    lifs_ctl();

    char buf[LIFS_BLOCKS][512] = {0};

    for(int i = 0; i < LIFS_BLOCKS; i++) memset(buf[i], 0, 512);

    for(int i = 0; i < LIFS_BLOCKS; i++)
        if(inod->blocks[i] != 0) ata_read_sector(inod->blocks[i], buf[i]);

    for(int i = 0; i < LIFS_BLOCKS; i++)
        for(int j = 0; j < 512; j++) buffer[(i * 512) + j] = buf[i][j];
}

void lifs_blk_iwrite(struct inode *inod, char *buffer, unsigned int amount, unsigned int blkadr) {
    char buf[512] = {0};

    lba_t lba = inod->blocks[blkadr];

    for(int i = 0; i < amount; i++) buf[i] = buffer[i];

    ata_write_sector(lba, buf);

    lifs_ctl();
}

void lifs_iwrite(struct inode *inod, unsigned int amount, char *buffer) {
    if(amount > LIFS_BLOCKS*512) panic("[lifs] Error. I dont rlly care enough to tell ya.\n");

    char buf1[512];
    char buf2[512];
    memset(buf1, 0, 512);
    memset(buf2, 0, 512);

    memcpy(buf1, buffer, amount);

    if(amount > 512) {
        memcpy(buf1, buffer, 512);
        buffer -= 512;
        memcpy(buf2, buffer, amount - 512);
    }

    lifs_blk_iwrite(inod, buf1, 512, 0);
    lifs_blk_iwrite(inod, buf2, 512, 1);
}


void lifs_read(char *name, char *buffer) {
    for(int i = 0; i < LIFS_MAX_FILES; i++) {
        if(strcmp(lifs_inodes[i].name, name) == 0) {
            lifs_iread(&lifs_inodes[i], buffer);
            return;
        }
    }

    printf("[lifs] Cannot find file: %s\n", name);
}

void lifs_write(char *name, unsigned int amount, char *buffer) {
    lifs_ctl();

    for(int i = 0; i < LIFS_MAX_FILES; i++) {
        if(strcmp(lifs_inodes[i].name, name) == 0) {
            lifs_iwrite(&lifs_inodes[i], amount, buffer);
            return;
        }
    }

    printf("[lifs] Cannot find file: %s\n", name);
}


void lifs_list(void) {
    for(int i = 0; i < LIFS_MAX_FILES; i++)
        if(strcmp(lifs_inodes[i].name, "") != 0) printf("%s\n", lifs_inodes[i].name);
}
