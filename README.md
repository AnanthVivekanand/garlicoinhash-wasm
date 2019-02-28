# garlicoinhash-wasm

Provides WebAssembly code to hash strings using the Allium energy-efficient hashing function.

## Usage

    importScripts('./allium.js');
    Module.onRuntimeInitialized = function() {
        var heap = Module.HEAPU8.buffer;
        var header = new Uint8Array(heap, Module._malloc(80), 80);
        var output = new Uint8Array(heap, Module._malloc(32), 32);
        header.set(hex2Buf( data )); Or set another buffer to the Uint8Array
        Module.ccall("allium_hash", "string", [], [header.byteOffset, output.byteOffset]);
        console.log(buf2Hex(output));
    }
        
