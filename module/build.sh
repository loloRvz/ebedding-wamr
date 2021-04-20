#!/bin/bash

/opt/wasi-sdk/bin/clang \
	-o module.wasm module.c \
	-O3 -z stack-size=8192 -nostdlib \
	-Wl,--initial-memory=65536 \
    -Wl,--strip-all,--no-entry \
    -Wl,--export=__heap_base,--export=__data_end \
    -Wl,--export=sum \
    -Wl,--import-memory 

wamrc -o module.aot module.wasm

tftp 192.168.0.224 -c put module.wasm
tftp 192.168.0.224 -c put module.aot
