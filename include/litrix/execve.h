struct litrix_bprm {
    char magic[4]; // XKE.
    char *prog;
};

#define _Nullable

int exec(void *adr);
int exec_bprm(struct litrix_bprm *bprm);

int execve(const char *pathname, char *const _Nullable argv[],
           char *const _Nullable envp[]);
