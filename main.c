
#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include "wasm_c_api.h"

#define own




int main(int argc, char *argv[]){
	
	//Initialise
	printf("Initialising...\n");
	wasm_engine_t* engine = wasm_engine_new();
	wasm_store_t* store = wasm_store_new(engine);
	
	
	//Load binary
	printf("Loading binary...\n");
	FILE* file = fopen("./module/module.aot", "rb");
	if (!file) {
	  printf("> Error opening module!\n"); return 1;
	}
	fseek(file, 0L, SEEK_END);
	size_t file_size = ftell(file);
	fseek(file, 0L, SEEK_SET);
	wasm_byte_vec_t binary;
	wasm_byte_vec_new_uninitialized(&binary, file_size);
	if (fread(binary.data, file_size, 1, file) != 1) {
	  printf("> Error loading module!\n"); return 1;
	}
	fclose(file);
	
	
	//Compile module
	printf("Compiling module...\n");
	own wasm_module_t* module = wasm_module_new(store, &binary);
	if (!module) {
		printf("> Error compiling module!\n"); return 1;
	}
	wasm_byte_vec_delete(&binary);
	
	
	// Instantiate.
	printf("Instantiating module...\n");
	own wasm_instance_t* instance =
	wasm_instance_new(store, module, NULL, NULL);
	if (!instance) {
		printf("> Error instantiating module!\n"); return 1;
	}
	
	
	//Extract eports
	printf("Extracting export...\n");
	own wasm_extern_vec_t exports;
	wasm_instance_exports(instance, &exports);
	if (exports.size == 0) {
		printf("> Error accessing exports!\n"); return 1;
	}
	const wasm_func_t* sum_func = wasm_extern_as_func(exports.data[0]);
	if (sum_func == NULL) {
		printf("> Error accessing export!\n"); return 1;
	}
    
    
    //Delete module
    wasm_module_delete(module);
    wasm_instance_delete(instance);
    
    
	//Call export
	printf("Calling export...\n");
	wasm_val_t args[2];
	args[0].kind = WASM_I32;
	args[0].of.i32 = 6;
	args[1].kind = WASM_I32;
	args[1].of.i32 = 9;
	wasm_val_t results[1];
	if (wasm_func_call(sum_func, args, results)) {
		printf("> Error calling function!\n"); return 1;
	}
    wasm_extern_vec_delete(&exports);
    
    
	// Print result.
	printf("Printing result...\n");
	printf("> %u\n", results[0].of.i32);
    
    
	//Shut down
    printf("Shutting down...\n");
    wasm_store_delete(store);
    wasm_engine_delete(engine);

}

