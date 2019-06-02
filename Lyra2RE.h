#ifndef LYRA2RE_H
#define LYRA2RE_H

#ifdef __cplusplus
extern "C" {
#endif

void lyra2re_hash(const char* input, char* output);
void lyra2re2_hash(const char* input, char* output);
void allium_hash(const char* input, char* output);
bool fulltest(const uint32_t *hash, const uint32_t *target);
int scanhash_allium(int thr_id, struct work *work, uint32_t max_nonce, uint64_t *hashes_done);

#ifdef __cplusplus
}
#endif

#endif
