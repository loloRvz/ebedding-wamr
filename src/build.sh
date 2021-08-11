 #!/bin/bash

/opt/wasi-sdk/bin/clang \
	-o mod.wasm module.c \
	-O3 -z stack-size=8192 -nostdlib \
	-Wl,--initial-memory=65536 \
    -Wl,--strip-all,--no-entry \
    -Wl,--export=sum \
    -Wl,--export=callcallback \
    -Wl,--export=__heap_base \
    
wamrc -o mod.aot mod.wasm

