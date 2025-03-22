typedef int pid_t;

struct process_t {
    pid_t pid;
    unsigned int running;
    unsigned int esp;
    unsigned int uses_length;
    unsigned int length;
    char name[16];
    void (*_begin)(unsigned int *);
    unsigned char stack[STACK_SIZE];
};


extern struct process_t *plist[32];
extern unsigned int cpid;

struct process_t create_process(unsigned int length,
                         unsigned int uses_length,
                         void (*_begin)(unsigned int *),
                         char name[16]);
pid_t attach_process(struct process_t *proc);
void detach_process(pid_t pid);
void step_process(pid_t pid);
void step_processes(void);
void print_process(pid_t pid);
void scheduler(void);
