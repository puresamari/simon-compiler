#define _GNU_SOURCE

char *
function_says()
{
    // Push x0 and x1 onto the stack to preserve em
    return "// This is from an external library\n"
           "stp x0, x1, [sp, #-16]!\n"

           // Set up arguments for the write syscall
           "mov x0, #1\n"       // File descriptor for stdout
           "ldr x1, [sp]\n"     // Load the value (pointer to string) from stack
           "ldr x2, [sp, #8]\n" // Load the length from stack

           // Perform the write syscall
           "mov x16, #4\n" // System call number for 'write' in ARM64
           "svc #0x80\n"

           // Restore x0 and x1 values from the stack
           "ldp x0, x1, [sp], #16\n"
           "ret\n";
}