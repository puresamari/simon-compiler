# Simon standard library

## Functions

- `says` saves the current x0 and x1 registers to the stack, sets up and performs a `write` system call to write data to standard output, and then restores the original `x0` and `x1` register values.
- `declares` declares a variable.