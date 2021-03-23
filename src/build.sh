#!/bin/bash

prog_name="$1"

/opt/wasi-sdk/bin/clang -O3 -nostdlib \
    -o $prog_name.wasm $prog_name.c \
    -Wl,--no-entry -Wl,--export=$prog_name
