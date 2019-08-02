###### A very very basic assembly-like language syntactically similar to that of the Little Man Computer.

**Note: This was created as a project to learn C. No guarantee is made for the safety, security or operation of this software. Do not use in production, unless self-audited beforehand. Feel free to create issues or pull requests if you see bad code.**

## Compiling

SimpleASM is distributed as a CMake project. You must have the `bison` and `flex` utilities installed.

```
> $ cd sasm
sasm> $ mkdir build
sasm/build> $ cd build
sasm/build> $ cmake ..
sasm/build> $ make sasm_run
```
