#include <litrix/stack.h>
#include <litrix/memory.h>
#include <litrix/stdout.h>

void stack_init(char *stack) {
    memset(stack, 0, STACK_SIZE);
}
