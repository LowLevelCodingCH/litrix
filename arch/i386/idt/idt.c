#include <litrix/i386/idt.h>
#include <litrix/syscall.h>
#include <litrix/typedef.h>
#include <litrix/stack.h>
#include <litrix/scheduler.h>
#include <litrix/stdout.h>
#include <litrix/pc.h>
#include <litrix/cpu.h>
#include <litrix/portio.h>

#define PIC_MASTER_CMD 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_CMD 0xA0
#define PIC_SLAVE_DATA 0xA1

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
    "[FAULT] Division by zero\n",
    "[INTER] Debug\n",
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    "[FAULT] Invalid opcode at: %d\n",
    RESV_ERROR,
    "[FATAL] Double fault",
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    "[FAULT] General protection fault\n",
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    RESV_ERROR,
    "[FATAL] Machine check",
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
    int eip = 0;
    asm("movl %%eax, %0" : "=a" (eip));

    //TODO: DOND WORK PLS FIXXXS
    eip = r_pc();

    printf(exception_msgs[0]);
    // only handles `int X = Y / 0;`
    //              (16 bytes)
    eip += 16;
    w_eax(eip);
    w_pc(1);
    return;
}

void invalid_opc(void) {
    int eip = 0;
    asm("movl %%eax, %0" : "=a" (eip));

    printf(exception_msgs[6], eip);
    // only handles `asm("ud2");` and some more like "syscall"....
    //              (2 bytes)
    eip += 2;
    w_eax(eip);
    w_pc(1);
    return;
}

void debug_exc(void) {
    printf(exception_msgs[1]);
    return;
}

void machine_check(void) {
    panic(exception_msgs[18]);
    while(1) asm("hlt");
}

void double_fault(void) {
    panic(exception_msgs[8]);
    while(1) asm("hlt");
}

void gen_prot_fault(void) {
    int eip = 0;
    asm("movl %%eax, %0" : "=a" (eip));

    printf(exception_msgs[13], eip);
    asm("hlt"); // though
    return;
}

void kbhnd(void) {
    printf("Keyboard interurpt\n");
    outb(0x20, 0x20);
    return;
}

void default_hnd(void);
void syscall_hand(void);
void kbd_hnd(void);

void exc0(void);
void exc1(void);
void exc6(void);
void exc8(void);
void exc13(void);
void exc18(void);

void pic_init_kbd(void) {
    outb(0x20, 0x11);
    outb(0x21, 0x20);
    outb(0x21, 0x04);
    outb(0x21, 0x01);

    outb(0xA0, 0x11);
    outb(0xA1, 0x28);
    outb(0xA1, 0x02);
    outb(0xA1, 0x01);

    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);
}

void pic_unmask_irq1() {
    unsigned char mask = inb(0x21);

    mask &= ~(1 << 1);

    outb(0x21, mask);
}

void i386_idt_initialize(void) {
    for(int i = 0; i < 256; i++)
        i386_idt_setgate(i, default_hnd, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);

    i386_idt_setgate(0x00, exc0,  0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);
    i386_idt_setgate(0x01, exc1,  0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);
    i386_idt_setgate(0x06, exc6,  0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);
    i386_idt_setgate(0x08, exc8,  0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);
    i386_idt_setgate(0x0d, exc13, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);
    i386_idt_setgate(0x12, exc18, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);

    i386_idt_setgate(0x20, syscall_hand, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_GATE_32BIT_INT);

    pic_init_kbd();
    pic_unmask_irq1();

    i386_idt_load(&idt_descriptor);
}
