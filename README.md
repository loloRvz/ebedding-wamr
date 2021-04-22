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
need to:
* Clone the [WAMR](https://github.com/bytecodealliance/wasm-micro-runtime)
repo and define its root as ```WAMR_DIR``` in your environment variables.
* Be able to [build WASM applications](https://github.com/bytecodealliance/wasm-micro-runtime/blob/main/doc/build_wasm_app.md):
	* Install [wasi-sdk](https://github.com/WebAssembly/wasi-sdk/releases)
and extracting the archive to default path ```/opt/wasi-sdk```.
	* Install [wamrc](https://github.com/bytecodealliance/wasm-micro-runtime#build-wamrc-aot-compiler),
the AoT compiler and add it to you PATH.
* Build [iwasm](https://github.com/bytecodealliance/wasm-micro-runtime/blob/main/doc/build_wamr.md)
with the WASM interpreter disabled and AoT enabled.

## Example

The current example WASM module contains the following functions:
* sum(), which takes to integer parameters and returns their sum
* callback(), which calls a C function 'callback_func' that was imported
from main.c to the module. 

## Building and Running

```
cd src
./build.sh
cd ..

make clean
make
./project
```




