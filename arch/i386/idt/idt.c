#include <litrix/i386/idt.h>
#include <litrix/syscall.h>
#include <litrix/stack.h>
#include <litrix/scheduler.h>
#include <litrix/stdout.h>

struct idt_entry idt_entries[256];

struct idt_desc idt_descriptor = {sizeof(idt_entries) - 1, idt_entries};

void i386_idt_load(struct idt_desc *idt_descr);

void i386_idt_setgate(int interrupt, void *base,
                      unsigned short seg_desc, unsigned char flags)
{
    idt_entries[interrupt].base_low = ((unsigned int)base) & 0xFFFF;
    idt_entries[interrupt].seg_selector = seg_desc;
    idt_entries[interrupt].resv = 0;
    idt_entries[interrupt].flags = flags;
    idt_entries[interrupt].base_high = ((unsigned int)base >> 16) & 0xFFFF;
}

void i386_idt_enablegate(int interrupt) {
    FLAG_SET(idt_entries[interrupt].flags, IDT_FLAG_PRESENT);
}

void i386_idt_disablegate(int interrupt) {
    FLAG_UNSET(idt_entries[interrupt].flags, IDT_FLAG_PRESENT);
}

char *exception_msgs[32] = {
    "Division by zero\n",
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    "Invalid opcode\n",
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
};

void div_by_zero(void) {
    panic(exception_msgs[0]);
    detach_process(cpid);
    return;
}

void invalid_opc(void) {
    panic(exception_msgs[6]);
    detach_process(cpid);
    return;
}

void default_hnd(void);
void syscall_hand(void);
void exc0(void);
void exc6(void);

void i386_idt_initialize(void) {
    for(int i = 0; i < 256; i++)
        i386_idt_setgate(i, default_hnd, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);

    i386_idt_setgate(0x0, exc0, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);
    i386_idt_setgate(0x6, exc6, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);
    i386_idt_setgate(0x80, syscall_hand, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);

    i386_idt_load(&idt_descriptor);
}
