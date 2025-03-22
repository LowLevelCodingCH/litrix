#include <litrix/stack.h>
#include <litrix/memory.h>
#include <litrix/stdout.h>

void stack_init(unsigned int stack_top, unsigned char silent) {
    memset((char*)(stack_top-STACK_SIZE), 0, STACK_SIZE);

    if(!silent)
        printf(LITRIX_LOG "[io::stack] Initialized Stack\n");
}
