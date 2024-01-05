This is a compiler for [simon lang](https://github.com/puresamari/simon) that compiles the simonlang code into an executable binary.

To compile you need to:
- Run the compiler:
```
$ compiler ./test.simon ./test-out-binary
```
- Then because i'm bad at coding you need to manually assemble the assembly: 
```
$ as -arch arm64 -o ./test-out-binary.o ./test-out-binary.s
````
- And to finish off, link the code and libraries: 
```
$ ld -arch arm64 -e _start -o ./test-out-binary ./test-out-binary.o -lSystem -syslibroot `xcrun -sdk macosx --show-sdk-path` -e _start -arch arm64
```

# Docs

- [Libraries](./docs/libraries.md)
- [Standard library / Simon Library](./libraries/simon/README.md)

TODO:
- Add a proper release notes and version management system
- Automatically create the dirs that are referenced in the params
- Instead of transpiling interpretation results into assembly files, it should
  - Compile the interpretation result into binary machine code
  - Package them into an object file (ELF) that can be linked directly
- Fix workflow for windows
- Implement the full language capabilities