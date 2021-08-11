
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "wasm_c_api.h"
#include "wasm_export.h"

#define own


own wasm_trap_t* callback_func(const wasm_val_t args[], wasm_val_t results[]){
	printf("Running callback function...\n");
	return NULL;
}


int main(int argc, char *argv[]){
	
	if(argc != 2){
		printf("> Error incorrect input!\nUsage: wafle wasm_file\n"); 
		return 1;
	}
	
	//Select run mode (.wasm/.aot)
	if(!strstr(argv[1],".wasm") && !strstr(argv[1],".aot")){
		printf("> Error file type not supported"); return 1;
	}
	
	//Initialise
	printf("Initialising...\n");
	wasm_engine_t* engine = wasm_engine_new();
	wasm_store_t* store = wasm_store_new(engine);
	
	
	//Load binary
	printf("Loading binary...\n");
	FILE* file = fopen(argv[1], "rb");
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
	
	
	// Create external print functions.
	printf("Creating callback...\n");
	own wasm_functype_t* call_type = wasm_functype_new_0_0();
	own wasm_func_t* call_func = wasm_func_new(store,call_type,callback_func);
	wasm_functype_delete(call_type);
	
	
	// Instantiate.
	printf("Instantiating module...\n");
	const wasm_extern_t* imports[] = { wasm_func_as_extern(call_func) };
	own wasm_instance_t* instance = wasm_instance_new(store, module, imports, NULL);
	if (!instance) {
		printf("> Error instantiating module!\n"); return 1;
	}
	wasm_func_delete(call_func);
	
	
	//Extract eports
	printf("Extracting export...\n");
	own wasm_exporttype_vec_t export_types;
	own wasm_extern_vec_t exports;
	wasm_module_exports(module, &export_types);
	wasm_instance_exports(instance, &exports);
	assert(exports.size == export_types.size);
	if (exports.size == 0) {
		printf("> Error accessing exports!\n"); return 1;
	}
	const wasm_func_t* sum_wasmfunc = wasm_extern_as_func(exports.data[1]);
	if (sum_wasmfunc == NULL) {
		printf("> Error accessing export!\n"); return 1;
	}
	const wasm_func_t* call_wasmfunc = wasm_extern_as_func(exports.data[2]);
	if (call_wasmfunc == NULL) {
		printf("> Error accessing export!\n");return 1;
	}
    
    
    //Delete module
    wasm_module_delete(module);
    wasm_instance_delete(instance);
    
    
	//Call sum() and print result
	printf("Calling sum()...\n");
	wasm_val_t args[2];
	args[0].kind = WASM_I32;
	args[0].of.i32 = 6;
	args[1].kind = WASM_I32;
	args[1].of.i32 = 9;
	wasm_val_t results[1];
	if (wasm_func_call(sum_wasmfunc, args, results)) {
		printf("> Error calling function!\n"); return 1;
	}
	printf("Printing result of sum()...\n");
	printf("> %u\n", results[0].of.i32);
	
	//Call callback()
	printf("Calling callback()...\n");
	if (wasm_func_call(call_wasmfunc, args, results)) {
		printf("> Error calling function!\n"); return 1;
	}
	
	
	//Shut down
    printf("Shutting down...\n");
    wasm_extern_vec_delete(&exports);
    wasm_store_delete(store);
    wasm_engine_delete(engine);

}




