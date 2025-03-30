struct idt_entry {
    unsigned short base_low;
    unsigned short seg_selector;
    unsigned char resv;
    unsigned char flags;
    unsigned short base_high;
} __attribute__((packed));

struct idt_desc {
    unsigned short limit;
    struct idt_entry *idtr;
} __attribute__((packed));

typedef enum {
    IDT_FLAG_GATE_TASK              = 0x5,
    IDT_FLAG_GATE_16BIT_INT         = 0x6,
    IDT_FLAG_GATE_16BIT_TRAP        = 0x7,
    IDT_FLAG_GATE_32BIT_INT         = 0xE,
    IDT_FLAG_GATE_32BIT_TRAP        = 0xF,

    IDT_FLAG_RING0                  = (0 << 5),
    IDT_FLAG_RING1                  = (1 << 5),
    IDT_FLAG_RING2                  = (2 << 5),
    IDT_FLAG_RING3                  = (3 << 5),

    IDT_FLAG_PRESENT                = 0x80,
} IDT_FLAGS;

#define FLAG_SET(x, flag) x |= (flag)
#define FLAG_UNSET(x, flag) x &= ~(flag)
#define RESV_ERROR "Reserved\n"

void i386_idt_load(struct idt_desc *idt_descr);
void i386_idt_setgate(int interrupt, void *base,
                      unsigned short seg_desc, unsigned char flags);
void i386_idt_enablegate(int interrupt);
void i386_idt_disablegate(int interrupt);
void i386_idt_initialize(void);
