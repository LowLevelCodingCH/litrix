#include <litrix/stdout.h>
#include <litrix/virtmem.h>
#include <litrix/memory.h>

void *vm_find_address(struct vm_map *map, void *address) {
    if(!map)
        return NULL;
    if(!address)
        return NULL;

    for(int i = 0; i < VM_MAP_AMOUNT; i++)
        if(map->addresses[i] == address)
            return map->phys_addresses[i];

    return NULL;
}

void vm_init_map(struct vm_map *map) {
    if(!map)
        return;

    for(int i = 0; i < VM_MAP_AMOUNT; i++) {
        map->addresses[i] = NULL;
        map->phys_addresses[i] = NULL;
    }
}
