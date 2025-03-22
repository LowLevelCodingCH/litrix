#include <litrix/stack.h>
#include <litrix/memory.h>
#include <litrix/stdout.h>

void stack_init(char *stack, unsigned char silent) {
    memset(stack, 0, STACK_SIZE);

    if(!silent)
        printf(LITRIX_LOG "[io::stack] Initialized Stack\n");
}
