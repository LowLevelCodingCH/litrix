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
        memcpy(fs_entries[i].data, data+32, 512-32);
    }
    clear();
}

void smfs_print_content(char name[32]) {
    for(int i = 0; i < SMFS__MAX_FILES; i++) {
        if(strcmp(fs_entries[i].name, name) == 0) {
            printf(fs_entries[i].data);
            return;
        }
    }
    printf(LITRIX_ERR "[io::smfs] No such file: %s\n", name);
}

void smfs_write_content(char name[32], char data[512-32]) {
    char whole_file[512] = {0};

    for(int i = 0; i < SMFS__MAX_FILES; i++) {
        if(strcmp(fs_entries[i].name, name) == 0) {
            memcpy(fs_entries[i].data, data, 512-32);
            
            for(int j = 0; j < 32; j++)
                whole_file[j] = fs_entries[i].name[j];
            for(int j = 0; j < 512-32; j++)
                whole_file[j+32] = fs_entries[i].data[j];

            ata_write_sector(i, whole_file);

            return;
        }
    }
    printf(LITRIX_ERR "[io::smfs] No such file: %s\n", name);
}
