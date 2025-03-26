#include <litrix/stack.h>
#include <litrix/stdout.h>
#include <litrix/memory.h>
#include <litrix/scheduler.h>

struct process_t *plist[32];
unsigned int cpid = 0;

struct process_t create_process(void (*_begin)(unsigned int *, pid_t),
                                char name[16]) {
    struct process_t p;
    p.running = 1;
    p._begin = _begin;
    p.esp = (unsigned int)((char*)(p.stack + STACK_SIZE));

    for(int i = 0; i < 16; i++)
        p.name[i] = name[i];

    //p.pid = cpid; // will be done in attach_process
    return p;
}

// See, this uses child-friendly names instead of linux's "kill"

pid_t attach_process(struct process_t *proc) {
    if(cpid >= 32) return -1;

    proc->pid = cpid;
    plist[proc->pid] = proc;
    cpid++;
    return proc->pid;
}

void detach_process(pid_t pid) {
    if(!plist[pid]) return;

    printf("[sched] Detached process: `%s` with pid %d\n", plist[pid]->name, plist[pid]->pid);

    memset((char*)plist[pid]->name, 0, 16);
    plist[pid]->esp = 0;
    plist[pid]->running = 0;
    plist[pid]->_begin = NULL;

    for(int i = 0; i < 32; i++) {
        if(i >= pid && i < 32) {
            plist[i] = plist[i + 1];
            plist[i]->pid = i;
        }
        if(i == 32)
	    plist[i] = NULL;
    }

    cpid--;
}

void detach_all(void) {
    for(int i = cpid; i > 0; i--) {
        detach_process(i);
    }
    detach_process(0);
}

void step_process(pid_t pid) {
    if(!plist[pid]) return;
    if(plist[pid]->running != 1) return;

    plist[pid]->_begin(&plist[pid]->esp, plist[pid]->pid);
}

void step_processes(void) {
    for(int i = 0; i < 32; i++)
        step_process(i);
}

void print_process(pid_t pid) {
    printf("pid[%d]:\n\tname: %s\n\tesp: %d\n\t_begin: %d\n\trunning: %d\n\n", plist[pid]->pid,
           plist[pid]->name, plist[pid]->esp, (unsigned int)(plist[pid]->_begin),
           plist[pid]->running);
}

void list_processes(void) {
    for(int i = 0; i < cpid; i++) {
        printf("pid[%d] : %s\n", plist[i]->pid, plist[i]->name);
    }
}

void sched(void) {
    step_processes();
}

void scheduler(void) {
    for(;;) sched();
}
