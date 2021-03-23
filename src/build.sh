#!/bin/bash

/opt/wasi-sdk/bin/clang -O3 -nostdlib \
    -o exports.wasm exports.c \
    -Wl,--no-entry \
    -Wl,--export=sum \
    -Wl,--export=add_one \
