#define PREVERR

#ifdef PREVERR
int __stack_chk_fail_local(void) {
    return 0;
}
#endif