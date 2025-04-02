#include <litrix/stack.h>
#include <litrix/stdout.h>
#include <litrix/fs/lifs.h>
#include <litrix/typedef.h>
#include <litrix/fs/wrap_inc.h>
#include <litrix/syscall.h>
#include <litrix/memory.h>
#include <litrix/execve.h>
#include <litrix/scheduler.h>

int exec(void *adr) {
    int (*_begin)(void) = adr;
    return _begin();
}

int exec_bprm(struct litrix_bprm *bprm) {
    if(bprm->magic[0] == 'X' &&
       bprm->magic[1] == 'K' &&
       bprm->magic[2] == 'E' &&
       bprm->magic[3] == '.')
    {
        return exec((void*)bprm->prog);
    } else {
        printf("[execve] No valid signature, aborting...\n");
    }

    return -1;
}

int execve(const char *pathname, char *const _Nullable argv[],
           char *const _Nullable envp[])
{
    char fildat[512 * LIFS_BLOCKS];
    memset(fildat, 0, 512 * LIFS_BLOCKS);

    int fd = open(pathname);

    read(fd, fildat);

    memcpy((void*)0, (void*)(fildat + 4), 512 * LIFS_BLOCKS);

    struct litrix_bprm bprm = {0};

    bprm.magic[0] = fildat[0];
    bprm.magic[1] = fildat[1];
    bprm.magic[2] = fildat[2];
    bprm.magic[3] = fildat[3];

    bprm.prog = (char*)(0);

    close(fd);

    return exec_bprm(&bprm);
}
