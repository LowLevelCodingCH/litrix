typedef struct {
    u32 esp, ebp, eip;
    u32 eax, ebx, ecx, edx;
    u32 esi, edi;
} context;

void save_context(context *ctx);
void load_context(context *ctx);
void sched(void);
