#!/bin/bash

/opt/wasi-sdk/bin/clang \
	-o test.wasm test.c \
	-O3 -z stack-size=8192 -nostdlib\
	-Wl,--initial-memory=65536 \
    -Wl,--strip-all,--no-entry,--allow-undefined \
    -Wl,--export=main,--export=__main_argc_argv \
    -Wl,--export=__heap_base,--export=__data_end \
    -Wl,--import-memory

wamrc -o test.aot test.wasm

tftp 192.168.0.224 -c put test.wasm
tftp 192.168.0.224 -c put test.aot

