#include <litrix/stdout.h>
#include <litrix/memory.h>

void heap_init(struct heap_t *heap, char *heap_adr) {
    if(!heap) return;
    if(!heap_adr) return;

    for(unsigned int i = 0; i < MEMAMOUNT; i++) {
        heap->blocks[i].size = 0;
        heap->blocks[i].free = 0;
        heap->blocks[i].ptr  = heap_adr + (i * 512);
    }
}

void *heap_malloc(unsigned int size, struct heap_t *heap) {
    if(!heap) return NULL;
    if(size > 512) return NULL;

    for(unsigned int i = 0; i < MEMAMOUNT; i++) {
        if(heap->blocks[i].free == 1) {
            heap->blocks[i].size = size;
            heap->blocks[i].free = 0;
            return heap->blocks[i].ptr;
        }
    }

    return NULL;
}

void heap_free(void *block, struct heap_t *heap) {
    struct memblock_t *full_block = (struct memblock_t*)(block - (sizeof(struct memblock_t) - (sizeof(void*))));
    full_block->free = 1;
}

unsigned char memcmp(void *p1, void *p2, unsigned int len) {
    for(unsigned int i = 0; i < len; i++) {
        if(((char*)p1)[i] != ((char*)p2)[i]) return 1;
    }

    return 0;
}

void memset(void *mem, char set, unsigned int len) {
    char* m = mem;

    while(len > 0) {
        *m = set;
        m++;
        len--;
    }
}

void *memcpy(void *dest, void *src, unsigned int n) {
    unsigned char *d = dest;
    const unsigned char *s = src;

    while (n--) {
        *d++ = *s++;
    }

    return dest;
}

// Actually MOVES the data. not copy
void *memmove(void *dest, void *src, unsigned int n) {
    memcpy(dest, src, n);
    memset(src, 0, n);
    return dest;
}
