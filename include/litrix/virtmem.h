#define VM_MAP_AMOUNT 1024

struct vm_map {
    void *addresses[VM_MAP_AMOUNT];
    void *phys_addresses[VM_MAP_AMOUNT];
};

void vm_init_map(struct vm_map *map, unsigned char silent);
void *vm_find_address(struct vm_map *map, void *address);
