#define NULL ((void*)0)
#define MEMAMOUNT (1024)
#define MEMBASE (2000000000) /* (0xfffffff) */

struct memblock_t {
    unsigned int size;
    unsigned char free : 1;
    char *ptr;
} __attribute__((packed));


struct heap_t {
    struct memblock_t blocks[MEMAMOUNT];
} __attribute__((packed));

void heap_init(struct heap_t *heap, char *heap_adr, unsigned char silent);
void *heap_malloc(unsigned int size, struct heap_t *heap);
void heap_free(void *block, struct heap_t *heap);
unsigned char memcmp(void *p1, void *p2, unsigned int len);
void memset(void *mem, char set, unsigned int len);
void *memcpy(void *dest, const void *src, unsigned int n);
