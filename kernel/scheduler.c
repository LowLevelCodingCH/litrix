#include <litrix/stack.h>
#include <litrix/cpu.h>
#include <litrix/pc.h>
#include <litrix/typedef.h>
#include <litrix/stdout.h>
#include <litrix/memory.h>
#include <litrix/scheduler.h>

void save_context(context *ctx) {
    asm("cli");

    asm("mov -4(%%ebp), %0" : "=a"(ctx->eip));
    printf("%d\n", ctx->eip);

    asm("mov %%esp, %0" : "=S"(ctx->esp));
    asm("mov %%ebp, %0" : "=r"(ctx->ebp));

    asm("mov %%eax, %0" : "=a"(ctx->eax));
    asm("mov %%ebx, %0" : "=b"(ctx->ebx));
    asm("mov %%ecx, %0" : "=c"(ctx->ecx));
    asm("mov %%edx, %0" : "=d"(ctx->edx));

    asm("mov %%esi, %0" : "=r"(ctx->esi));
    asm("mov %%edi, %0" : "=r"(ctx->edi));

    asm("sti");
}

void load_context(context *ctx) {
    asm("cli");

    asm("mov %0, %%esp" : : "S"(ctx->esp));
    asm("mov %0, %%ebp" : : "r"(ctx->ebp));

    asm("mov %0, %%eax" : : "a"(ctx->eax));
    asm("mov %0, %%ebx" : : "b"(ctx->ebx));
    asm("mov %0, %%ecx" : : "c"(ctx->ecx));
    asm("mov %0, %%edx" : : "d"(ctx->edx));

    asm("mov %0, %%esi" : : "r"(ctx->esi));
    asm("mov %0, %%edi" : : "r"(ctx->edi));

    asm("sti");

    w_eax(ctx->eip);
    w_pc(1);
}

void sched(void) {
}
