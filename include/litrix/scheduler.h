typedef int pid_t;

struct process_t {
    pid_t pid;
    unsigned int running;
    unsigned int esp;
    char name[16];
    void (*_begin)(unsigned int *, pid_t);
    unsigned char stack[STACK_SIZE];
};

extern struct process_t *plist[32];
extern unsigned int cpid;

struct process_t create_process(void (*_begin)(unsigned int *, pid_t),
                                char *name);
pid_t attach_process(struct process_t *proc);
void detach_process(pid_t pid);
int fork_process(pid_t pid);
void detach_all(void);
void step_process(pid_t pid);
void step_processes(void);
void print_process(pid_t pid);
void list_processes(void);
void sched(void);
void scheduler(void);
