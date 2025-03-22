#include <litrix/stack.h>
#include <litrix/stdout.h>
#include <litrix/memory.h>
#include <litrix/scheduler.h>

struct process_t *plist[32];
unsigned int cpid = 0;

struct process_t create_process(unsigned int length,
                         unsigned int uses_length,
                         void (*_begin)(unsigned int *),
                         char name[16]) {
    struct process_t p;
    p.uses_length = uses_length;
    p.length = length;
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

    printf("[io::sched] Detached task: `%s` with pid %d\n", plist[pid]->name, plist[pid]->pid);

    memset((char*)plist[pid]->name, 0, 16);
    plist[pid]->length = 0;
    plist[pid]->esp = 0;
    plist[pid]->running = 0;
    plist[pid]->_begin = NULL;

    for(int i = 0; i < 32; i++) {
        if(i >= pid && i < 32)
            plist[i] = plist[i + 1];
        if(i == 32)
	    plist[i] = NULL;
    }

    cpid--;
}

void step_process(pid_t pid) {
    if(!plist[pid]) return;
    if(plist[pid]->running != 1) return;

    plist[pid]->_begin(&plist[pid]->esp);
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

void scheduler(void) {
    step_processes();
}
