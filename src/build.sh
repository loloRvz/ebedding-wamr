 #!/bin/bash

/opt/wasi-sdk/bin/clang \
	-o mod.wasm module.c \
	-O3 -z stack-size=8192 -nostdlib -mbulk-memory \
	-Wl,--initial-memory=65536 \
    -Wl,--strip-all,--no-entry \
    -Wl,--export=sum \
    -Wl,--export=callcallback \
    -Wl,--export=__heap_base \
    #-Wl,--import-memory
    
#clang++ module.c -ObjC++ --compile --target=wasm32-unknown-unknown-wasm --optimize=3 -o test.wasm 

wasm2wat -o mod.wat mod.wasm
wamrc -o mod.aot mod.wasm

wat2wasm -o sum.wasm sum.wat
wamrc  -o sum.aot sum.wasm


