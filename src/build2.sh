#!/bin/bash
    
/opt/wasi-sdk/bin/clang -O3 -nostdlib -pthread\
	-z stack-size=8192 -Wl,--initial-memory=65536 \
    -o module.wasm module.c \
    -Wl,--shared-memory,--max-memory=65536 \
    -Wl,--no-entry,--export=__heap_base,--export=__data_end\
    -Wl,--export=sum \
    -Wl,--export=add_one \

wamrc -o module.aot module.wasm
