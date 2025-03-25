#include <litrix/stdout.h>
#include <litrix/memory.h>
#include <litrix/disk.h>
#include <litrix/fs/lifs.h>

/*
 * Litrix FS
 *        File System
 */


struct inode lifs_inodes[LIFS_MAX_FILES] = {0};

void lifs_ctl(void) {
    char buf[512] = {0};

    for(int i = 0; i < LIFS_MAX_FILES; i++) {
        ata_read_sector(i, buf);
        
        struct inode inod = {0};

        inod.size = ((unsigned int*)(buf))[8];
        for(int j = 0; j < 8; j++)
            inod.blocks[j] = ((lba_t*)(buf))[j];
        for(int j = 0; j < 128; j++)
            inod.name[j] = buf[((sizeof(lba_t)*8)+sizeof(unsigned int))+j];
        
        lifs_inodes[i].size = inod.size;
        memset(lifs_inodes[i].resv, 0, 348);

        for(int j = 0; j < 8; j++)
            lifs_inodes[i].blocks[j] = inod.blocks[j];
        for(int j = 0; j < 128; j++)
            lifs_inodes[i].name[j] = inod.name[j];
    }
}

void lifs_iread(struct inode *inod, char buf[8][512]) {
    for(int i = 0; i < 8; i++)
        if(inod->blocks[i] != 0) ata_read_sector(inod->blocks[i], buf[i]);
}

void lifs_read(char name[128], char buf[8][512]) {
    for(int i = 0; i < LIFS_MAX_FILES; i++) {
        if(strcmp(lifs_inodes[i].name, name) == 0) {
            for(int j = 0; j < 8; j++)
                if(lifs_inodes[i].blocks[j] != 0) ata_read_sector(lifs_inodes[i].blocks[j], buf[j]);
	}
    }
}
