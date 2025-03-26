#include <litrix/memory.h>
#include <litrix/disk.h>
#include <litrix/stdout.h>
#include <litrix/fs/smfs.h>

// Simple file system designed by llcch (me)
// Every file is in 1 block (512 bytes) and cant grow bigger
// The first 32 of which are the file name
// This makes it easier to read and write, but harder to use
// So this is a trade off and a quick test fs
// To see if everything is working correctly
// Max 100 files
// If you make an OS, implement SMFS first to see if everything works
// Theory is easy (I made it in 1 minute)
// Practically it also is easy (I implemented it in 3 minutes)

struct smfs_entr fs_entries[SMFS__MAX_FILES];

void smfs_ctl(void) {
    char data[512];

    for(int i = 0; i < SMFS__MAX_FILES; i++) {
        ata_read_sector(i, data);

        memcpy(fs_entries[i].name, data, 32);
        memcpy(fs_entries[i].data, data + 32, 512 - 32);
    }
}

int smfs_read(char name[32], char *buf) {
    for(int i = 0; i < SMFS__MAX_FILES; i++) {
        if(strcmp(fs_entries[i].name, name) == 0) {
            for(int j = 0; j < 512 - 32; j++)
                buf[j] = fs_entries[i].data[j];

            return 0;
        }
    }
    printf("[smfs] No such file: %s\n", name);
    return -1;
}

int smfs_test(char name[32]) {
    for(int i = 0; i < SMFS__MAX_FILES; i++)
        if(strcmp(fs_entries[i].name, name) == 0)
            return -1;

    return 0;
}

int smfs_write(char name[32], char data[512-32]) {
    char whole_file[512] = {0};

    for(int i = 0; i < SMFS__MAX_FILES; i++) {
        if(strcmp(fs_entries[i].name, name) == 0) {
            memcpy(fs_entries[i].data, data, 512-32);
            
            for(int j = 0; j < 32; j++)
                whole_file[j] = fs_entries[i].name[j];
            for(int j = 0; j < 512-32; j++)
                whole_file[j+32] = fs_entries[i].data[j];

            ata_write_sector(i, whole_file);

            return 0;
        }
    }
    printf("[smfs] No such file: %s\n", name);
    return -1;
}

int smfs_rename(char name[32], char newname[32]) {
    char whole_file[512] = {0};

    char data[512-32] = {0};

    smfs_read(name, data);

    for(int i = 0; i < SMFS__MAX_FILES; i++) {
        if(strcmp(fs_entries[i].name, name) == 0) {
            memcpy(fs_entries[i].name, newname, 32);
            memcpy(fs_entries[i].data, data, 512-32);

            for(int j = 0; j < 32; j++)
                whole_file[j] = fs_entries[i].name[j];
            for(int j = 0; j < 512-32; j++)
                whole_file[j+32] = fs_entries[i].data[j];

            ata_write_sector(i, whole_file);

            return 0;
        }
    }

    return -1;
}

int smfs_creat(char name[32]) {
    char buf[512] = {0};
    memset(buf, 0, 512);

    if(smfs_test(name) != 0) return -1;
    smfs_rename("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", name);
    smfs_write(name, buf);
    return 0;
}

void smfs_list(void) {
    printf("ID  Name\n");
    for(int i = 0; i < SMFS__MAX_FILES; i++)
        if(strcmp(fs_entries[i].name, "") != 0)
            printf("%d\t%s\n", i, fs_entries[i].name);
}
