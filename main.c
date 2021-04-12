
#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include "wasm_c_api.h"
#include "wasm_export.h"
#include "lib_export.h"
#include "aot_export.h"

#define own

#define WASM_EMPTY_VEC {0, NULL}
#define WASM_ARRAY_VEC(array) {sizeof(array)/sizeof(*(array)), array}

#define WASM_I32_VAL(i) {.kind = WASM_I32, .of = {.i32 = i}}
#define WASM_I64_VAL(i) {.kind = WASM_I64, .of = {.i64 = i}}
#define WASM_F32_VAL(z) {.kind = WASM_F32, .of = {.f32 = z}}
#define WASM_F64_VAL(z) {.kind = WASM_F64, .of = {.f64 = z}}
#define WASM_REF_VAL(r) {.kind = WASM_ANYREF, .of = {.ref = r}}
#define WASM_INIT_VAL {.kind = WASM_ANYREF, .of = {.ref = NULL}}


int main(){
	
	//Initialise
	printf("Initialising...\n");
	wasm_engine_t* engine = wasm_engine_new();
	wasm_store_t* store = wasm_store_new(engine);
	
	
	//Load binary
	printf("Loading binary...\n");
	FILE* file = fopen("./src/module.wasm", "rb");
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
	
	
	//Instantiate module
	printf("Instantiating module...\n");
	wasm_extern_vec_t imports = WASM_EMPTY_VEC;
	own wasm_instance_t* instance = wasm_instance_new(store, module, &imports, NULL);
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
    const wasm_func_t* sum_func = wasm_extern_as_func(exports.data[1]);
    if (sum_func == NULL) {
		printf("> Error accessing exported function sum!\n"); return 1;
    }
    const wasm_func_t* add_one_func = wasm_extern_as_func(exports.data[2]);
    if (add_one_func == NULL) {
		printf("> Error accessing exported function add_one!\n"); return 1;
    }
    
    
    //Delete module
    wasm_module_delete(module);
    wasm_instance_delete(instance);
    
    
	//Call function sum
    printf("Calling function...\n");
    wasm_val_t args_val[] = { WASM_I32_VAL(5), WASM_I32_VAL(11) };
    wasm_val_t results_val[1] = { WASM_INIT_VAL };
    wasm_val_vec_t args = WASM_ARRAY_VEC(args_val);
    wasm_val_vec_t results = WASM_ARRAY_VEC(results_val);

    if (wasm_func_call(sum_func, &args, &results)) {
        printf("> Error calling function!\n"); return 1;
    }
    printf("Results: %d\n", results_val[0].of.i32);
    

    //Call function add_one
    printf("Calling function...\n");
    wasm_val_t args_val2[] = { WASM_I32_VAL(5) };
    wasm_val_vec_t args2 = WASM_ARRAY_VEC(args_val2);

    if (wasm_func_call(add_one_func, &args2, &results)) {
        printf("> Error calling function!\n"); return 1;
    }
    printf("Results: %d\n", results_val[0].of.i32);

    wasm_extern_vec_delete(&exports);
    wasm_store_delete(store);
    wasm_engine_delete(engine);


}

