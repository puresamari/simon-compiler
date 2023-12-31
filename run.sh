# Create a build directory (recommended to keep the source directory clean)
mkdir build
cd build

# Generate build files with CMake
cmake ..

# Build the project
make

# Test build

rm -rf ./dist
mkdir ./dist
./bin/compiler ../test/test.simon ./dist/test-out-binary

# Assemble the code
as -arch arm64 -o ./dist/test-out-binary.o ./dist/test-out-binary.s -g

# Link the code
ld -arch arm64 -e _start -o ./dist/test-out-binary ./dist/test-out-binary.o -lSystem -syslibroot `xcrun -sdk macosx --show-sdk-path` -e _start -arch arm64

echo "\n\nExecuting....\n"
./dist/test-out-binary
