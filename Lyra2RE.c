/*-
 * Copyright 2009 Colin Percival, 2011 ArtForz, 2013 Neisklar, 2014 James Lovejoy
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file was originally written by Colin Percival as part of the Tarsnap
 * online backup system.
 */

#include <emscripten.h>

#include "Lyra2RE.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "crypto/sha3/sph_blake.h"
#include "crypto/sha3/sph_groestl.h"
#include "crypto/sha3/sph_cubehash.h"
#include "crypto/sha3/sph_bmw.h"
#include "crypto/sha3/sph_keccak.h"
#include "crypto/sha3/sph_skein.h"
#include "crypto/Lyra2.h"

EMSCRIPTEN_KEEPALIVE

int main(int argc, char ** argv) {}

void allium_hash(const char* input, char* state)
{
    uint32_t hashA[8], hashB[8];

    sph_blake256_context     ctx_blake;
    sph_keccak256_context    ctx_keccak;
    sph_skein256_context     ctx_skein;
    sph_groestl256_context   ctx_groestl;
    sph_cubehash256_context  ctx_cube;

    // sph_blake256_set_rounds(14);

    sph_blake256_init(&ctx_blake);
    sph_blake256(&ctx_blake, input, 80);
    sph_blake256_close(&ctx_blake, hashA);

    sph_keccak256_init(&ctx_keccak);
    sph_keccak256(&ctx_keccak, hashA, 32);
    sph_keccak256_close(&ctx_keccak, hashB);

    LYRA2_old(hashA, 32, hashB, 32, hashB, 32, 1, 8, 8);

    sph_cubehash256_init(&ctx_cube);
    sph_cubehash256(&ctx_cube, hashA, 32);
    sph_cubehash256_close(&ctx_cube, hashB);

    LYRA2_old(hashA, 32, hashB, 32, hashB, 32, 1, 8, 8);

    sph_skein256_init(&ctx_skein);
    sph_skein256(&ctx_skein, hashA, 32);
    sph_skein256_close(&ctx_skein, hashB);

    sph_groestl256_init(&ctx_groestl);
    sph_groestl256(&ctx_groestl, hashB, 32);
    sph_groestl256_close(&ctx_groestl, hashA);

    memcpy(state, hashA, 32);
}

void lyra2re_hash(const char* input, char* output)
{
    sph_blake256_context     ctx_blake;
    sph_groestl256_context   ctx_groestl;
    sph_keccak256_context    ctx_keccak;
    sph_skein256_context     ctx_skein;

    uint32_t hashA[8], hashB[8];

    sph_blake256_init(&ctx_blake);
    sph_blake256 (&ctx_blake, input, 80);
    sph_blake256_close (&ctx_blake, hashA);	
	
    sph_keccak256_init(&ctx_keccak);
    sph_keccak256 (&ctx_keccak,hashA, 32); 
    sph_keccak256_close(&ctx_keccak, hashB);
	
	LYRA2_old(hashA, 32, hashB, 32, hashB, 32, 1, 8, 8);
	
	sph_skein256_init(&ctx_skein);
    sph_skein256 (&ctx_skein, hashA, 32); 
    sph_skein256_close(&ctx_skein, hashB); 

    sph_groestl256_init(&ctx_groestl);
    sph_groestl256 (&ctx_groestl, hashB, 32); 
    sph_groestl256_close(&ctx_groestl, hashA); 

	memcpy(output, hashA, 32);
}

void lyra2re2_hash(const char* input, char* output)
{
	sph_blake256_context ctx_blake;
	sph_cubehash256_context ctx_cubehash;
	sph_keccak256_context ctx_keccak;
	sph_skein256_context ctx_skein;
	sph_bmw256_context ctx_bmw;
	
	uint32_t hashA[8], hashB[8];
	
	sph_blake256_init(&ctx_blake);
    sph_blake256(&ctx_blake, input, 80);
    sph_blake256_close (&ctx_blake, hashA);	
	
    sph_keccak256_init(&ctx_keccak);
    sph_keccak256(&ctx_keccak, hashA, 32); 
    sph_keccak256_close(&ctx_keccak, hashB);
    
    sph_cubehash256_init(&ctx_cubehash);
    sph_cubehash256(&ctx_cubehash, hashB, 32);
    sph_cubehash256_close(&ctx_cubehash, hashA);
    
    LYRA2(hashB, 32, hashA, 32, hashA, 32, 1, 4, 4);
    
   	sph_skein256_init(&ctx_skein);
    sph_skein256(&ctx_skein, hashB, 32); 
    sph_skein256_close(&ctx_skein, hashA);
    
    sph_cubehash256_init(&ctx_cubehash);
    sph_cubehash256(&ctx_cubehash, hashA, 32);
    sph_cubehash256_close(&ctx_cubehash, hashB);
    
    sph_bmw256_init(&ctx_bmw);
    sph_bmw256(&ctx_bmw, hashB, 32);
    sph_bmw256_close(&ctx_bmw, hashA);
    
   	memcpy(output, hashA, 32);
}

static inline void be32enc(void *pp, uint32_t x)
{
	uint8_t *p = (uint8_t *)pp;
	p[3] = x & 0xff;
	p[2] = (x >> 8) & 0xff;
	p[1] = (x >> 16) & 0xff;
	p[0] = (x >> 24) & 0xff;
}



bool fulltest(const uint32_t *hash, const uint32_t *target)
{
	int i;
	bool rc = true;
	
	for (i = 7; i >= 0; i--) {
		if (hash[i] > target[i]) {
			rc = false;
			break;
		}
		if (hash[i] < target[i]) {
			rc = true;
			break;
		}
	}

	return rc;
}


int scanhash_allium(int thr_id, struct work *work, uint32_t max_nonce, uint64_t *hashes_done)
{
	uint32_t _ALIGN(128) hash[8];
	uint32_t _ALIGN(128) endiandata[20];
	uint32_t *pdata = work->data;
	uint32_t *ptarget = work->target;

	const uint32_t Htarg = ptarget[7];
	const uint32_t first_nonce = pdata[19];
	uint32_t n = first_nonce;

	for (int i=0; i < 19; i++) {
		be32enc(&endiandata[i], pdata[i]);
	}

	do {
		be32enc(&endiandata[19], n);
		allium_hash(hash, endiandata);

		if (hash[7] < Htarg && fulltest(hash, ptarget)) {
			//work_set_target_ratio(work, hash);
			*hashes_done = n - first_nonce + 1;
			pdata[19] = n;
			return 1;
		}
		n++;

	} while (n < max_nonce);

	*hashes_done = n - first_nonce + 1;
	pdata[19] = n;

	return 0;
}
