# Libraries

For now libraries are just .dylib files.

### Functions

Functions in simon libs require a prefix named `function_[function_name]`, each function should just return the assembly code it should execute.

#### Assembly

The funciton will receive the args with the register increasing by one, so for example
`simon says test`
would load the library `libraries/libsimon.dylib` and call the function `function_says` to get the assembly code. once the code is executed it will load the text `test` into the `x0` register