###### A very very basic assembly-like language syntactically similar to that of the Little Man Computer.

**Note: This was created as a project to learn C. No guarantee is made for the safety, security or operation of this software. Do not use in production, unless self-audited beforehand. Feel free to create issues or pull requests if you see bad code.**

## Compiling

SimpleASM is distributed as a CMake project. You need to have `bison` and `flex` installed. To generate the Makefile, clone and run:

`$ cmake .`

Then run `make sasm_run` to compile.
