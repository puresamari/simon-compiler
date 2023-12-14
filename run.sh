rm -rf ./dist
mkdir ./dist

clang ./compiler/compiler.c -o ./dist/compiler

./dist/compiler ./test/test.simon ./dist/test-out-binary

# Assemble the code
as -arch arm64 -o ./dist/test-out-binary.o ./dist/test-out-binary.s

# Link the code
ld -arch arm64 -e _start -o ./dist/test-out-binary ./dist/test-out-binary.o -lSystem -syslibroot `xcrun -sdk macosx --show-sdk-path` -e _start -arch arm64

echo "\n\nExecuting....\n"
./dist/test-out-binary
