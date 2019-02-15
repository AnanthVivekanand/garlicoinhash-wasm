#!/bin/bash

/home/raptor/emsdk/emscripten/1.38.15/emcc Lyra2RE.c crypto/*.c crypto/sha3/*.c -O3 \
    -s DISABLE_EXCEPTION_CATCHING=1 \
    -s BINARYEN_ASYNC_COMPILATION=1 \
    -s ALIASING_FUNCTION_POINTERS=0 \
    -s ALLOW_MEMORY_GROWTH=0 \
    -s VERBOSE=0 \
    -s WASM=1 \
    -s BINARYEN=1 \
    -s NO_EXIT_RUNTIME=1 \
    -s ASSERTIONS=0 \
    -s SAFE_HEAP=1 \
    -s STACK_OVERFLOW_CHECK=0 \
    -s EXPORTED_FUNCTIONS="['_allium_hash']" \
    -s EXTRA_EXPORTED_RUNTIME_METHODS="['ccall']" \
    -o ./allium.js
