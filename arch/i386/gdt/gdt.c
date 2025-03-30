#include <litrix/i386/gdt.h>

struct gdt_entry gdt_entries[3] = {
    GDT_ENTRY(0, 0, 0, 0),

    GDT_ENTRY(0,
              0xFFFFF,
              GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_CODE_SEGMENT | GDT_ACCESS_CODE_READABLE,
              GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K),

    GDT_ENTRY(0,
              0xFFFFF,
              GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_DATA_SEGMENT | GDT_ACCESS_DATA_WRITEABLE,
              GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K),
};

struct gdt_desc gdt_descriptor = {sizeof(gdt_entries) - 1, gdt_entries};

void i386_gdt_load(struct gdt_desc *descriptor, unsigned short code_seg, unsigned short data_seg);

void i386_gdt_initialize(void) {
    asm volatile("ltr %0" :: "r"(0));
    i386_gdt_load(&gdt_descriptor, i386_GDT_CODE_SEGMENT, i386_GDT_DATA_SEGMENT);
}
