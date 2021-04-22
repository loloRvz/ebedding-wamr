# Embedding WAMR

This repo documents the progress of running wasm modules from a 'native' 
C program. In this example, 'main.c' is our C program and module.aot, is 
the wasm module, which is written in C, then compiled to the AoT wasm
format.

## Prerequisites

To build the C program you'll need the essential C-programming tools:

```
sudo apt-get install gcc build-essential
```

If you'd like to make modifications to the wasm module and rebuild it you'll 
need to have properly installed [WAMR](https://github.com/bytecodealliance/wasm-micro-runtime) 



## Building and Running

```
cd src
./build.sh
cd ..

make clean
make
./project
```


