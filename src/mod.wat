(module
  (type (;0;) (func))
  (type (;1;) (func (param i32 i32) (result i32)))
  (import "env" "callback" (func (;0;) (type 0)))
  (func (;1;) (type 1) (param i32 i32) (result i32)
    local.get 1
    local.get 0
    i32.add)
  (func (;2;) (type 0)
    call 0)
  (table (;0;) 1 1 funcref)
  (memory (;0;) 1)
  (global (;0;) (mut i32) (i32.const 9216))
  (global (;1;) i32 (i32.const 9216))
  (export "memory" (memory 0))
  (export "sum" (func 1))
  (export "callcallback" (func 2))
  (export "__heap_base" (global 1)))
