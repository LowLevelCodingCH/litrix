#define IVT_LEN 256

struct ivt {
    unsigned int vectors[IVT_LEN];
} __attribute__((packed));