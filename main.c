#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "wasm.h"

int main(){
	
	//Open and load module
	FILE* file = fopen("./src/exports.wasm", "rb");
	if (!file) {
	  printf("> Error opening module!\n"); return 1;
	}
	fseek(file, 0L, SEEK_END);
	size_t file_size = ftell(file);
	fseek(file, 0L, SEEK_SET);
	wasm_byte_vec_t wasm_bytes;
	wasm_byte_vec_new_uninitialized(&wasm_bytes, file_size);

	if (fread(wasm_bytes.data, file_size, 1, file) != 1) {
	  printf("> Error loading module!\n"); return 1;
	}
	fclose(file);
	

	//Compile module
	wasm_engine_t* engine = wasm_engine_new();
	wasm_store_t* store = wasm_store_new(engine);
	wasm_module_t* module = wasm_module_new(store, &wasm_bytes);

	if (!module) {
		printf("> Error compiling module!\n"); return 1;
	}
	
	
	//Instantiating module
	wasm_extern_vec_t imports = WASM_EMPTY_VEC;
	wasm_instance_t* instance = wasm_instance_new(store, module, &imports, NULL);

	if (!instance) {
		printf("> Error instantiating module!\n"); return 1;
	}
	
	
	//Retrieve eports
	wasm_extern_vec_t exports;
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

/*
int main(){
	
	// Read the Wasm file bytes.
	FILE *file = fopen("./src/sum.wasm", "r");
	if(!file){printf("Error opening file! Exiting...\n");exit(1);}
	fseek(file, 0, SEEK_END);
	long len = ftell(file);
	uint8_t *bytes = malloc(len);
	fseek(file, 0, SEEK_SET);
	fread(bytes, 1, len, file);
	fclose(file);

	// Prepare the imports.
	wasmer_import_t imports[] = {};

	// Instantiate!
	wasmer_instance_t *instance = NULL;
	wasmer_result_t instantiation_result = wasmer_instantiate(&instance, bytes, len, imports, 0);

	assert(instantiation_result == WASMER_OK);

	// Let's call a function.
	// Start by preparing the arguments.

	// Value of argument #1 is `7i32`.
	wasmer_value_t argument_one;
	argument_one.tag = WASM_I32;
	argument_one.value.I32 = 7;

	// Value of argument #2 is `8i32`.
	wasmer_value_t argument_two;
	argument_two.tag = WASM_I32;
	argument_two.value.I32 = 8;

	// Prepare the arguments.
	wasmer_value_t arguments[] = {argument_one, argument_two};

	// Prepare the return value.
	wasmer_value_t result_one;
	wasmer_value_t results[] = {result_one};

	// Call the `sum` function with the prepared arguments and the return value.
	wasmer_result_t call_result = wasmer_instance_call(instance, "sum", arguments, 2, results, 1);

	// Let's display the result.
	printf("Call result:  %d\n", call_result);
	printf("Result: %d\n", results[0].value.I32);

	// `sum(7, 8) == 15`.
	assert(results[0].value.I32 == 15);
	assert(call_result == WASMER_OK);

	wasmer_instance_destroy(instance);

	return 0;
} 
*/
