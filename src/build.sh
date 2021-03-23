#!/bin/bash

/opt/wasi-sdk/bin/clang -O3 -nostdlib -pthread\
    -o exports.wasm exports.c \
    -Wl,--shared-memory,--max-memory=131072 \
    -Wl,--no-entry,--export=__heap_base,--export=__data_end\
    -Wl,--export=sum \
    -Wl,--export=add_one \
