# Interpreter

### Line interpretation

- 1. The interpreter extracts the library from the first part of the instruction
- 2. It checks if the library is added to the library hashmap already
  - 2a. If the library is not added to the library hashmap yet it tries to find it (this part isn't yet figured out) and add it to the hashmap
- 3. It extract the function from the second parameter of the instruction
- 4. It checks if the function has been added to the functions hashmap
  - 4a. If the function isn't added yet it checks if it is avaliable in the referenced library and adds it to the hashmap
- 5. It loads the compiler for the used function and adds it to a list of compilers
- 6. It adds the instruction to a list of instructions (the instruction contains the referenced function hash and the parameters)